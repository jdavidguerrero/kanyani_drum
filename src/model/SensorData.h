#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include "utils/ads1115/Ads1115.h"

class SensorData : public QObject
{
    Q_OBJECT

public:
    explicit SensorData(QObject *parent = nullptr);
    int readSensor(int sensorIndex);
    int getSensorCount() const { return 8; }

private:
    ControllerADS1115 m_adsController; // Instancia de la clase ControllerADS1115
};

#endif // SENSORDATA_H
