#include <Arduino.h>
#include <GPS.h>

GPSData gdata;

GPSData GetGPSData() {
    Serial.printf("GetGPSData()");
    return gdata;
}
