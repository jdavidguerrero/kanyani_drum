#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <QSettings>
#include "model/SensorData.h"
#include "utils/ads1115/Ads1115.h"
#include "audio/AudioModule.h"

const int CALIBRATION_SAMPLES = 50;
const int SAMPLING_INTERVAL = 1; // Adjust this value as needed
const int FILTER_SIZE = 2;       // Size of the moving average filter
const int NUM_SENSORS = 5;       // Adjust according to the actual number of sensors
const int PEAK_THRESHOLD = 10;
const int DEBOUNCE_TIME = 100;

class SensorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QString> samples READ getSamples WRITE setSamples NOTIFY samplesChanged)
    Q_PROPERTY(QVector<int> sensorValues READ getSensorValues NOTIFY sensorValuesChanged)
    Q_PROPERTY(QVector<int> thresholds READ getThresholds WRITE setThresholds NOTIFY thresholdsChanged)

public:
    explicit SensorController(SensorData *sensorData, QObject *parent = nullptr);
    ~SensorController();
    QVector<int> getSensorValues() const { return m_sensorValues; }
    QVector<int> getThresholds() const;
    void setThresholds(const QVector<int> &thresholds);
    QVector<QString> getSamples() const;
    void setSamples(const QVector<QString> &samples);
    void calibrateSensors();
    Q_INVOKABLE void setSampleForChannel(const QString &filename, int channel);
    Q_INVOKABLE QString getSampleForChannel(int channel) const;
    Q_INVOKABLE void startReading();
    Q_INVOKABLE void stopReading();
signals:
    void sensorValuesChanged();
    void thresholdsChanged();
    void thresholdExceeded(int channel);
    void samplesChanged();

private:
    void readSensors();
    void processSensorData();
    void initSensorStates();
    int process_value(int16_t rawValue);
    bool detectPeak(int sensorIndex, int16_t value);
    int16_t applyMovingAverageFilter(int sensorIndex, int16_t newValue);
    bool debounce(int sensorIndex);
    void saveThresholdSettings();
    void loadThresholdSettings();
    void loadSettings();
    void saveSettings() const;

    SensorData *m_sensorData;
    bool m_isReading;
    QVector<bool> m_calibrated;
    QVector<int> m_calibrationCounts;
    QVector<int> m_restValues;
    QVector<int> m_sensorValues;
    QVector<int> m_thresholds;
    QVector<QString> m_samples;
    QVector<std::deque<int16_t>> sensorValuesHistory;
    QVector<bool> sensorStates;
    std::queue<std::pair<int, int16_t>> sensorDataQueue;
    std::mutex queueMutex;
    std::condition_variable dataCondition;
    std::thread sensorThread;
    std::thread processingThread;
    AudioModule audioModule;

    struct SensorState
    {
        int lastValue = 0;
        bool isStable = false;
        std::chrono::steady_clock::time_point lastTriggerTime;
        std::deque<int16_t> history;
    };

    std::vector<SensorState> m_sensorStates;
};

#endif // SENSORCONTROLLER_H
