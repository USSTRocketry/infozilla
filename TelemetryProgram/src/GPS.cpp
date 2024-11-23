#include <Arduino.h>
#include <GPS.h>

GPSData data;

GPSData GetGPSData() {
    Serial.printf("GetGPSData()");
    return data;
}
