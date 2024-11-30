#include <Arduino.h>
#include <GPS.h>

<<<<<<< HEAD
gpsData data;

gpsData GetGPSData() {
    Serial.printf("GetGPSData()");
    return data;
=======
GPSData gdata;

GPSData GetGPSData() {
    Serial.printf("GetGPSData()");
    return gdata;
>>>>>>> main
}
