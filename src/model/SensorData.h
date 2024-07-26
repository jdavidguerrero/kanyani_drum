#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <vector>
#include "utils/ads1115/Ads1115.h" // Asegúrate de incluir el archivo adecuado para ADS1115

class SensorData
{
public:
    SensorData();
    int readSensor(int sensorIndex); // Función para leer el valor del sensor
private:
    ADS1115 m_ads1115; // Instancia de la clase ADS1115
    // Aquí podrías tener otros datos y funciones privadas
};

#endif // SENSORDATA_H
