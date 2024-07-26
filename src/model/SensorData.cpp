#include "SensorData.h"

SensorData::SensorData()
{
    // Inicialización de tus datos, si es necesario
}

int SensorData::readSensor(int sensorIndex)
{
    // Aquí deberías implementar la lógica para leer el sensor correspondiente
    // Por ejemplo, si estás utilizando ADS1115, deberías llamar a la función adecuada para leer el valor del sensor
    // Supongamos que tienes una instancia de ADS1115 llamada `m_ads1115`
    return m_ads1115.readADC_SingleEnded(sensorIndex);
}
