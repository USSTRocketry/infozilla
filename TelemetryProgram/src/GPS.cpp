#include <Arduino.h>
#include <GPS.h>

gpsData data;

gpsData GetGPSData() {
    Serial.printf("GetGPSData()");
    return data;
}
