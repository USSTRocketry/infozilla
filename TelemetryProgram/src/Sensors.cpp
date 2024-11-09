#include <Arduino.h>
#include <SensorStructs.h>

SensorData data;

void InitSensors(){
    Serial.printf("InitSensors()");
}



struct SensorData GetSensorData(){
    Serial.printf("GetSensorData()");
    return data;
}