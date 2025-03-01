#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <SensorStructs.h>

SensorData sdata;

/**
 * Every sensor library that we need:
 * 
 * 1) barometer bmp280
 * 
 * 2) accelerometer kx134-1211 -- need to find on github
 * 
 * 3) magnetometer mmc5603nj
 * 
 * 4) FRAM: MB85RC256VPF-G-JNERE2
 * 
 * 5) Radio: Rfm95w - not confident in which lib
 * 
 * 6) gyro: NA
 * 
 * 7) Thermocouple: max 31855
 * 
 */

void InitSensors(){
    Serial.printf("InitSensors()");
}

SensorData* GetSensorData(){

    return &sdata;
}

void GetBarData(){

}