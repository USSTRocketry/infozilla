#include <Arduino.h>
// #include <DataPackager.h>
// #include <DataStorage.h>
// #include <GPS.h>
// #include <Radio.h>
// #include <SDHandler.h>
// #include <Sensors.h>
// #include <SensorStructs.h>
#include <Wire.h>
#include "Avionics_HAL.h"

SensorBMP280 bmp280Sensor;

void setup() {
    Serial.begin(9600);
    if (bmp280Sensor.begin()) {
        Serial.println("BMP280 initialized.");
    } else {
        Serial.println("Initialization failed.");
    }
}

void loop() {
    BMP280Data* data = bmp280Sensor.read();
    Serial.print("Temp: ");
    Serial.print(data->temperature);
    Serial.print("°C, Pressure: ");
    Serial.print(data->pressure);
    Serial.print("Pa, Altitude: ");
    Serial.print(data->altitude);
    Serial.println("m");
    delay(1000);
}

// char data[sizeof(SensorData)];

// int i = 0;
// SensorData *sensorData;
// GPSData gpsData;
// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   Serial.begin(0);
//   // put your setup code here, to run once:
//   // int result = myFunction(2, 3);
  
//   InitDataPackager();
//   InitDataStorage();
//   InitRadio();
//   InitSDHandler();
//   InitSensors();
// }

// void loop() {
//   i++;
//   Serial.printf("Hello World: %i\n", i);
//   delay(500);
//   // put your main code here, to run repeatedly:
//   sensorData = GetSensorData();
//   gpsData = GetGPSData();

// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }

// void Handledata(){

// }

// void TransferDataToSD(){

// }

// char *TurnSensorDataToBytes(SensorData *data){
//   return NULL;

// }