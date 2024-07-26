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
#include "SensorData.h"
#include "AudioModule.h"
#include "utils/ads1115/Ads1115.h"

const int CALIBRATION_SAMPLES = 100;
const int SAMPLING_INTERVAL = 10; // Adjust this value as needed
const int FILTER_SIZE = 5;        // Size of the moving average filter
const int NUM_SENSORS = 5;        // Adjust according to the actual number of sensors
const int PEAK_THRESHOLD = 100;
const int DEBOUNCE_THRESHOLD = 3;

class SensorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qvector<int> thresholds READ getThresholds WRITE setThresholds NOTIFY thresholdsChanged)

public:
    explicit SensorController(SensorData *sensorData, QObject *parent = nullptr);
    ~SensorController();

    QVector<int> getThresholds() const;
    void setThresholds(const QVector<int> &newThresholds);

signals:
    void sensorValuesChanged();
    void thresholdsChanged();

public slots:
    void startReading();
    void stopReading();
    void calibrateSensors();
    void setSampleForChannel(const QString &filename, int channel);
    QString getSampleForChannel(int channel) const;

private:
    void readSensors();
    void processSensorData();
    int process_value(int16_t rawValue);
    bool detectPeak(int sensorIndex, int16_t value);
    void printSensorValue(int sensorIndex, int16_t value);
    int16_t applyMovingAverageFilter(int sensorIndex, int16_t newValue);
    bool debounce(int sensorIndex, int16_t value);
    void saveThresholdSettings();
    void loadThresholdSettings();
    void initSensorStates();
    void loadSettings();
    void saveSettings();

    SensorData *m_sensorData;
    bool m_isReading;
    QVector<std::deque<int16_t>> sensorValuesHistory;
    QVector<int16_t> restValues;
    QVector<int> m_thresholds;
    QVector<bool> sensorStates;
    std::queue<std::pair<int, int16_t>> sensorDataQueue;
    std::mutex queueMutex;
    std::condition_variable dataCondition;
    std::thread sensorThread;
    std::thread processingThread;
    AudioModule audioModule;

    struct SensorState
    {
        int lastValue;
        int stableCount;
        bool isStable;
    };

    std::vector<SensorState> m_sensorStates;
};

#endif // SENSORCONTROLLER_H
