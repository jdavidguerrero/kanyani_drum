#include "AudioModule.h"
#include <sndfile.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

AudioModule::AudioModule()
{
    if (dac.getDeviceCount() < 1)
    {
        std::cerr << "No audio devices found!\n";
        exit(1);
    }
    setupAudioStream();
}

AudioModule::~AudioModule()
{
    if (dac.isStreamOpen())
        dac.closeStream();
}

void AudioSample::loadFromFile(const std::string &filename)
{
    this->filename = filename;
    SF_INFO sfinfo;
    SNDFILE *file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file)
    {
        std::cerr << "Can't open file: " << filename << std::endl;
        std::cerr << "Error: " << sf_strerror(file) << std::endl;
        return;
    }

    size_t numSamples = sfinfo.frames * sfinfo.channels;
    data.resize(numSamples);

    sf_read_float(file, data.data(), numSamples);
    sf_close(file);
}

void AudioModule::setupAudioStream()
{
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    std::cout << "DeviceId: " << parameters.deviceId << std::endl;
    parameters.nChannels = 2; // stereo
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 512;

    try
    {
        std::cout << "Configuring audio stream..." << std::endl;
        dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &AudioModule::audioCallback, this);
        if (dac.isStreamOpen())
        {
            std::cout << "Audio Stream Opened." << std::endl;
        }
        else
        {
            std::cerr << "Error opening AudioStream." << std::endl;
        }
    }
    catch (RtAudioError &e)
    {
        std::cerr << "ERROR open stream: " << e.getMessage() << std::endl;
        exit(1);
    }
    catch (std::exception &e)
    {
        std::cerr << "Standard error: " << e.what() << std::endl;
        exit(1);
    }
    catch (...)
    {
        std::cerr << "Unknown error" << std::endl;
        exit(1);
    }
}

void AudioModule::setVolumeChannel(size_t channel, int volumeLevel)
{
    if (channel < samples.size())
    {
        samples[channel].setVolume(volumeLevel);
    }
}

void AudioModule::loadSampleForChannel(const std::string &filename, size_t channel)
{
    std::cout << "Loading sample for channel " << channel << " from file: " << filename << std::endl;

    if (channel >= samples.size())
    {
        samples.resize(channel + 1);
    }

    samples[channel].loadFromFile(filename);
    samples[channel].channel = channel;

    if (samples[channel].data.empty())
    {
        std::cerr << "Error: Failed to load sample for channel " << channel << " from file: " << filename << std::endl;
    }
    else
    {
        std::cout << "Sample loaded successfully for channel " << channel << std::endl;
    }
}

void AudioModule::playSample(size_t channel)
{
    if (channel < samples.size())
    {
        samples[channel].reset();
        if (std::find(activeChannels.begin(), activeChannels.end(), channel) == activeChannels.end())
        {
            activeChannels.push_back(channel);
        }
        if (!dac.isStreamRunning())
        {
            try
            {
                dac.startStream();
            }
            catch (RtAudioError &error)
            {
                error.printMessage();
            }
        }
    }
}
std::string AudioModule::getSampleForChannel(size_t channel) const
{
    if (channel < samples.size())
    {
        return samples[channel].filename;
    }
    return "";
}

int AudioModule::audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                               double streamTime, RtAudioStreamStatus status, void *userData)
{
    AudioModule *audioModule = static_cast<AudioModule *>(userData);
    float *out = static_cast<float *>(outputBuffer);

    std::fill_n(out, nBufferFrames * 2, 0);

    for (size_t channel : audioModule->activeChannels)
    {
        if (channel >= audioModule->samples.size())
            continue;

        AudioSample &sample = audioModule->samples[channel];
        if (sample.frameCounter >= sample.data.size())
            continue;

        float volume = sample.volume;

        for (unsigned int i = 0; i < nBufferFrames; ++i)
        {
            if (sample.frameCounter < sample.data.size())
            {
                out[i * 2] += sample.data[sample.frameCounter++] * volume;                                                                      // Left Channel
                out[i * 2 + 1] += sample.data[sample.frameCounter < sample.data.size() ? sample.frameCounter++ : sample.frameCounter] * volume; // Right Channel
            }
        }
    }

    audioModule->activeChannels.erase(std::remove_if(audioModule->activeChannels.begin(), audioModule->activeChannels.end(),
                                                     [audioModule](size_t channel)
                                                     {
                                                         return audioModule->samples[channel].frameCounter >= audioModule->samples[channel].data.size();
                                                     }),
                                      audioModule->activeChannels.end());

    return 0;
}
