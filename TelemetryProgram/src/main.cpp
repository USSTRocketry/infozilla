// Setup code to run only once
#include <Arduino.h>
#include <Wire.h>
#include <SDHandler.h>
#include <SensorInit.h>

// HAL libs
#include "Avionics_HAL.h"


void setup() {

    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect.
    }
    
}

void loop() {
//   i++;
//   Serial.printf("Hello World: %i\n", i);
//   delay(500);
//   // put your main code here, to run repeatedly:
//   sensorData = GetSensorData();
//   gpsData = GetGPSData();
    
}

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

