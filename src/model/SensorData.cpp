#include "SensorData.h"

SensorData::SensorData(QObject *parent)
    : QObject(parent)
{
    // Aquí puedes inicializar cualquier otra cosa que necesites
}

int SensorData::readSensor(int sensorIndex)
{
    int device = sensorIndex / 4; // Asumiendo que tienes 2 dispositivos y 4 canales por dispositivo
    int channel = sensorIndex % 4;
    int16_t value = m_adsController.readADC_SingleEnded(device, channel);
    return value;
}
