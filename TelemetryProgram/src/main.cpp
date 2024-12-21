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
#include <SDHandler.h>


// char data[sizeof(SensorData)];

// int i = 0;
// SensorData *sensorData;
// GPSData gpsData;
// // put function declarations here:
// int myFunction(int, int);

void setup() {
    // Serial.begin(0);
// put your setup code here, to run once:
// int result = myFunction(2, 3);
  
//   InitDataPackager();
//   InitDataStorage();
//   InitRadio();
//   InitSDHandler();
//   InitSensors();
    InitSD();
}

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