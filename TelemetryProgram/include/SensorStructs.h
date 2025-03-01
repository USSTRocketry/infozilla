#pragma once

typedef struct {
    float x;
    float y;
    float z;
}AxelData;

typedef struct 
{
    float x;
    float y;
    float z;
}GyroData;


typedef struct 
{
    /* data */
    float barval;//32
    float thermoval;//32
    AxelData accelerometer;//32*3
    GyroData gyroscopeData;//32*3
    //GPSData GPS;
    long int timestamp;
}SensorData;



