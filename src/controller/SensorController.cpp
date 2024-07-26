#include "controller/SensorController.h"
#include <QSettings>
#include <QStandardPaths>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <queue>

SensorController::SensorController(SensorData *sensorData, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_isReading(false), sensorValuesHistory(NUM_SENSORS), restValues(NUM_SENSORS, 0), m_thresholds(NUM_SENSORS, 1000), sensorStates(NUM_SENSORS)
{
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        sensorValuesHistory[i] = std::deque<int16_t>(FILTER_SIZE, 0); // Asignar deque a QVector de deques
    }
    initSensorStates();
}

SensorController::~SensorController()
{
    stopReading();
}

void SensorController::startReading()
{
    m_isReading = true;
    sensorThread = std::thread(&SensorController::readSensors, this);
    processingThread = std::thread(&SensorController::processSensorData, this);
}

void SensorController::stopReading()
{
    m_isReading = false;
    if (sensorThread.joinable())
        sensorThread.join();
    if (processingThread.joinable())
        processingThread.join();
}

void SensorController::calibrateSensors()
{
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        int sum = 0;
        for (int j = 0; j < CALIBRATION_SAMPLES; ++j)
        {
            int value = m_sensorData->readSensor(i);
            sum += value;
            std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLING_INTERVAL));
        }
        restValues[i] = sum / CALIBRATION_SAMPLES;
    }
}

void SensorController::readSensors()
{
    while (m_isReading)
    {
        for (int i = 0; i < NUM_SENSORS; ++i)
        {
            int16_t value = m_sensorData->readSensor(i) - restValues[i];
            std::lock_guard<std::mutex> lock(queueMutex);
            sensorDataQueue.push(std::make_pair(i, value));
            dataCondition.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLING_INTERVAL));
    }
}

void SensorController::processSensorData()
{
    while (m_isReading)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        dataCondition.wait(lock, [this]
                           { return !sensorDataQueue.empty(); });
        while (!sensorDataQueue.empty())
        {
            auto [sensorIndex, value] = sensorDataQueue.front();
            sensorDataQueue.pop();
            lock.unlock();

            value = applyMovingAverageFilter(sensorIndex, value);
            if (detectPeak(sensorIndex, value))
            {
                if (debounce(sensorIndex, value))
                {
                    printSensorValue(sensorIndex, value);
                    audioModule.playSample(sensorIndex);
                }
            }

            lock.lock();
        }
    }
}

int SensorController::process_value(int16_t rawValue)
{
    return std::abs(rawValue);
}

bool SensorController::detectPeak(int sensorIndex, int16_t value)
{
    return std::abs(value) > m_thresholds[sensorIndex];
}

void SensorController::printSensorValue(int sensorIndex, int16_t value)
{
    std::cout << "Sensor " << sensorIndex << ": " << value << std::endl;
}

int16_t SensorController::applyMovingAverageFilter(int sensorIndex, int16_t newValue)
{
    auto &history = sensorValuesHistory[sensorIndex];
    history.push_back(newValue);
    if (history.size() > FILTER_SIZE)
        history.pop_front();

    int sum = 0;
    for (int16_t val : history)
        sum += val;

    return sum / FILTER_SIZE;
}

bool SensorController::debounce(int sensorIndex, int16_t value)
{
    auto &state = m_sensorStates[sensorIndex];
    if (std::abs(value - state.lastValue) < PEAK_THRESHOLD)
    {
        state.stableCount++;
        if (state.stableCount >= DEBOUNCE_THRESHOLD)
        {
            state.isStable = true;
        }
    }
    else
    {
        state.stableCount = 0;
        state.isStable = false;
    }
    state.lastValue = value;
    return state.isStable;
}

void SensorController::initSensorStates()
{
    m_sensorStates.resize(NUM_SENSORS);
    for (auto &state : m_sensorStates)
    {
        state = {0, 0, false};
    }
}

void SensorController::saveThresholdSettings()
{
    std::ofstream file("thresholds.txt");
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        file << m_thresholds[i] << std::endl;
    }
}

void SensorController::loadThresholdSettings()
{
    std::ifstream file("thresholds.txt");
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        file >> m_thresholds[i];
    }
}

void SensorController::setSampleForChannel(const QString &filename, int channel)
{
    audioModule.loadSampleForChannel(filename.toStdString(), channel);
}

QString SensorController::getSampleForChannel(int channel) const
{
    return QString::fromStdString(audioModule.getSampleForChannel(channel));
}

void SensorController::loadSettings()
{
    QSettings settings("MyCompany", "MyApp");
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        m_thresholds[i] = settings.value(QString("thresholds/channel%1").arg(i), 1000).toInt();
        audioModule.loadSampleForChannel(settings.value(QString("samples/channel%1").arg(i)).toString().toStdString(), i);
    }
}

void SensorController::saveSettings()
{
    QSettings settings("MyCompany", "MyApp");
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        settings.setValue(QString("thresholds/channel%1").arg(i), m_thresholds[i]);
        settings.setValue(QString("samples/channel%1").arg(i), QString::fromStdString(audioModule.getSampleForChannel(i)));
    }
}

void SensorController::setThresholds(const QVector<int> &newThresholds)
{
    if (m_thresholds != newThresholds)
    {
        m_thresholds = newThresholds;
        emit thresholdsChanged();
    }
}
