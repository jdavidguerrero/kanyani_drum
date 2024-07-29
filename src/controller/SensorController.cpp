#include "controller/SensorController.h"
#include <QSettings>
#include <QStandardPaths>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <queue>
#include <QDebug>

SensorController::SensorController(SensorData *sensorData, QObject *parent)
    : QObject(parent),
      m_sensorValues(NUM_SENSORS, 0),
      m_sensorData(sensorData),
      m_isReading(false),
      sensorValuesHistory(NUM_SENSORS),
      m_calibrationCounts(NUM_SENSORS, 0),
      m_restValues(NUM_SENSORS, 0),
      m_calibrated(NUM_SENSORS, false),
      m_thresholds(NUM_SENSORS, 1000),
      sensorStates(NUM_SENSORS),
      m_samples(NUM_SENSORS)
{
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        sensorValuesHistory[i] = std::deque<int16_t>(FILTER_SIZE, 0); // Asignar deque a QVector de deques
        audioModule.setVolumeChannel(i, 100);
    }
    m_samples.resize(NUM_SENSORS);
    loadSettings();
    initSensorStates();
}

SensorController::~SensorController()
{
    stopReading();
}

void SensorController::startReading()
{
    qDebug() << "Starting to read sensors...";
    m_isReading = true;
    sensorThread = std::thread(&SensorController::readSensors, this);
    processingThread = std::thread(&SensorController::processSensorData, this);
}

void SensorController::stopReading()
{
    qDebug() << "Stopping sensor reading...";
    m_isReading = false;
    if (sensorThread.joinable())
        sensorThread.join();
    if (processingThread.joinable())
        processingThread.join();
}

void SensorController::readSensors()
{
    while (m_isReading)
    {
        for (int i = 0; i < NUM_SENSORS; ++i)
        {
            int16_t value = m_sensorData->readSensor(i);
            if (value == -1)
                continue;

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                sensorDataQueue.push(std::make_pair(i, value));
            }
            dataCondition.notify_one();

            if (!m_calibrated[i])
            {
                m_calibrationCounts[i]++;
                m_restValues[i] += value;
                if (m_calibrationCounts[i] == CALIBRATION_SAMPLES)
                {
                    m_restValues[i] /= CALIBRATION_SAMPLES;
                    m_calibrated[i] = true;
                    qDebug() << "Sensor" << i << "calibrated with baseline" << m_restValues[i];
                }
            }
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

            value -= m_restValues[sensorIndex];

            value = applyMovingAverageFilter(sensorIndex, value);
            if (value > m_thresholds[sensorIndex] && debounce(sensorIndex))
            {

                qDebug() << "Sensor" << sensorIndex << "triggered!!";
                audioModule.playSample(sensorIndex);
                m_sensorValues[sensorIndex] = value;
                emit sensorValuesChanged();
                emit thresholdExceeded(sensorIndex);
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

int16_t SensorController::applyMovingAverageFilter(int sensorIndex, int16_t newValue)
{
    auto &history = m_sensorStates[sensorIndex].history;

    // Agregar el nuevo valor al final de la historia
    history.push_back(newValue);

    // Si la historia excede el tamaño del filtro, eliminar el valor más antiguo
    if (history.size() > FILTER_SIZE)
    {
        history.pop_front();
    }

    // Calcular el promedio de los valores en la historia
    int sum = 0;
    for (int16_t value : history)
    {
        sum += value;
    }

    // Retornar el valor promedio
    return sum / static_cast<int16_t>(history.size());
}
bool SensorController::debounce(int sensorIndex)
{
    auto &state = m_sensorStates[sensorIndex];
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - state.lastTriggerTime).count();

    if (elapsed > DEBOUNCE_TIME)
    {
        state.lastTriggerTime = now;
        return true;
    }

    return false;
}
void SensorController::initSensorStates()
{
    m_sensorStates.resize(NUM_SENSORS);
    for (auto &state : m_sensorStates)
    {
        state = SensorState();
    }
}

void SensorController::setSampleForChannel(const QString &filename, int channel)
{
    QString localFilePath = filename;
    if (localFilePath.startsWith("file:///"))
    {
        localFilePath = localFilePath.mid(8);
    }

    if (channel >= 0 && channel < m_samples.size())
    {
        m_samples[channel] = localFilePath;
        audioModule.loadSampleForChannel(localFilePath.toStdString(), channel);
        emit samplesChanged();
        saveSettings();
    }
}

QVector<QString> SensorController::getSamples() const
{
    return m_samples;
}
void SensorController::setSamples(const QVector<QString> &samples)
{
    if (m_samples != samples)
    {
        m_samples = samples;
        emit samplesChanged();
    }
}

QString SensorController::getSampleForChannel(int channel) const
{
    if (channel >= 0 && channel < NUM_SENSORS)
    {
        return m_samples[channel];
    }
    return QString();
}

void SensorController::loadSettings()
{
    QSettings settings("GPROG", "Kanyani");

    // Convertir QVariantList a QVector<int>
    QList<QVariant> thresholdList = settings.value("thresholds").toList();
    if (thresholdList.size() == NUM_SENSORS)
    {
        for (int i = 0; i < NUM_SENSORS; ++i)
        {
            m_thresholds[i] = thresholdList[i].toInt();
        }
    }
    else
    {
        m_thresholds.fill(1000, NUM_SENSORS);
    }

    // Convertir QStringList a QVector<QString>
    QStringList sampleList = settings.value("samples").toStringList();
    if (sampleList.size() == NUM_SENSORS)
    {
        for (int i = 0; i < NUM_SENSORS; ++i)
        {
            m_samples[i] = sampleList[i];
        }
    }
    else
    {
        m_samples.fill("", NUM_SENSORS);
    }
}

void SensorController::saveSettings() const
{
    QSettings settings("GPROG", "Kanyani");

    // Convertir QVector<int> a QList<QVariant>
    QList<QVariant> thresholdList;
    for (int threshold : m_thresholds)
    {
        thresholdList.append(threshold);
    }
    settings.setValue("thresholds", thresholdList);

    // Convertir QVector<QString> a QStringList
    QStringList sampleList;
    for (const auto &sample : m_samples)
    {
        sampleList.append(sample);
    }
    settings.setValue("samples", sampleList);
}

void SensorController::setThresholds(const QVector<int> &thresholds)
{
    if (m_thresholds != thresholds)
    {
        m_thresholds = thresholds;
        emit thresholdsChanged();
    }
}

QVector<int> SensorController::getThresholds() const
{
    return m_thresholds;
}

void SensorController::saveThresholdSettings()
{
    QSettings settings;
    for (int i = 0; i < m_thresholds.size(); ++i)
    {
        settings.setValue(QString("thresholds/channel%1").arg(i), m_thresholds[i]);
    }
}

void SensorController::loadThresholdSettings()
{
    QSettings settings;
    for (int i = 0; i < m_thresholds.size(); ++i)
    {
        m_thresholds[i] = settings.value(QString("thresholds/channel%1").arg(i), 1000).toInt();
    }
}
