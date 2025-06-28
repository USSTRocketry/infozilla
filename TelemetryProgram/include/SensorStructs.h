#pragma once
#include <types.h>

typedef struct 
{
    /* data */
    BMP280Data bmp280;
    AccelGyroData accelGyro;
    MagnetometerData magnetometer;
    GPSData gps;
    float temperature;
    long int timestamp;
}SensorData;



