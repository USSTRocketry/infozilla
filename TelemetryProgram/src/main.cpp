// Setup code to run only once
#include <Arduino.h>
#include <Wire.h>
#include <SDHandler.h>
#include <SensorInit.h>
#include <Sensors.h>

// HAL libs
#include "Avionics_HAL.h"

// Low Power mode libraries
#include <ArduinoLowPower.h>
#include <FlexiTimer2.h>

// Enter low power mode
void sleep() {
    //go in low power mode. Note that the board will reset once it is woken up
    LowPower.deepSleep();
}

void setup() {

    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect.
    }
    //InitSensors();
    InitSD();
    WriteTest();
}

void loop() {
//   i++;
//   Serial.printf("Hello World: %i\n", i);
//   delay(500);
//   // put your main code here, to run repeatedly:
//   sensorData = GetSensorData();
//   gpsData = GetGPSData();

    PrintSensors();
    //Create delay so sensord Read everything at same rate probably want to set to 100 ms for actual flight.
    delay(1000);

}

// void Handledata(){

// }

// void TransferDataToSD(){

// }

// char *TurnSensorDataToBytes(SensorData *data){
//   return NULL;

// }

