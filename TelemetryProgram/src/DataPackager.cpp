/**
 * @file DataPackager.cpp
 * @author USST Avionics
 * @brief This file determines the state of flight of the rocket and base height of the rocket
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Arduino.h>
// #include <DataPackager.h>
#include <fp16.h>
#include <SensorStructs.h>
#include <DataStorage.h>
#include <DataPackager.h>
#include <Sensors.h>
#include <SDHandler.h>
#include <string>

//this define is for the low power library needing this define but not having it defined only sems to be an issue on erics computer.
#ifndef voidFuncPtr
#define voidFuncPtr void*
#endif

#include <ArduinoLowPower.h>
#include <FlexiTimer2.h>

#define IsTestingMode true

#if IsTestingMode
#define TestingMultiplier 0.01
#else
#define TestingMultiplier 1
#endif

FlightStates FlightState = PowerUp;
double InitialHeightData;
double barvalQueue[30];
int BarvalStart = 0, BarvalEnd = 1;
uint32_t startTimeToSleep = 0;
bool isCollectingData = true;

void InitDataPackager(){
    //Serial.printf("InitDataPackager()");
    InitialHeightData = GetSensorData()->bmp280.altitude;
    
}

void AddValToQueue(double val){
    BarvalStart = (BarvalStart+1)%30;
    BarvalEnd = (BarvalEnd+1)%30;
    barvalQueue[BarvalStart] = val;
}

double ReadValFromQueue(){
    return barvalQueue[BarvalEnd];
}

void TransferToSD(){
    Serial.printf("TransferToSD()");
}

void HandleData(SensorData *RetrivedData) {

    if(isCollectingData){
        Update(RetrivedData->bmp280.altitude);
        StoreStringLine(convertDataToCSVRow(RetrivedData));
    }else{
        Update(1013.25); //sea level
    }
}

FlightStates GetFlightState(){
    return FlightState;
}

void SetFlightState(FlightStates flightState){
    FlightState = flightState;
}

/**
 * @brief State machine for the flight of the rocket
 * 
 * @param barval_meters 
 */
void Update(float barval_meters){
    
    double currentBarVal = barval_meters;
    AddValToQueue(currentBarVal);
    switch (FlightState)
    {
    case PowerUp:
        /* dont do anything */
        isCollectingData = false;

        break;
    case ReadyForLaunch:
        isCollectingData = true;
        if(abs(InitialHeightData-currentBarVal) > 5.0 * TestingMultiplier){
            FlightState = InFlight;
        }

        break;
    case InFlight:
        if(abs(ReadValFromQueue()-currentBarVal) < 1.0){
            startTimeToSleep = millis();
            FlightState = Landed;
        }
        break;
    case Landed:
        // Wait about 60 seconds
        if(millis() - startTimeToSleep > 60000){
            FlightState = SaveData;
            isCollectingData = false;
        }
        
        break;
    case SaveData:
        // todo: move all files from builtin flash to sd

        FlightState = PowerDown;
        break;
    case PowerDown:
        // go to low power state 
        // LowPower.deepSleep();
        // LowPower.deepSleep(5000);
        break;
    default:
        break;
    }
}
