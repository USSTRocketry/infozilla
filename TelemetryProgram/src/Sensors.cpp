#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <SensorStructs.h>
#include <SensorInit.h>
#include <SDHandler.h>

// HAL libs
#include "Avionics_HAL.h"

SensorData sdata;

// Constructors for sensors
SensorBMP280 bmp280Sensor; // SensorBMP280 - Temp, pressure, and altitude
SensorAccelGyro accelGyro(0x6A, I2C_WIRE0); // Accelerometer and gyroscope sensor Default I2C address and bus
SensorMagnetometer magnetometer(LIS3MDL_I2CADDR_DEFAULT, I2C_WIRE0); // Magnometer Default I2C address and bus
SensorTemperature tempSensor; // Default I2C address and bus
GPS gps(GPS_HW_SERIAL, 9600);
// RYLR998Radio radio(HW_SERIAL8, 9600); // Not default Radio to be used
RFM95Radio radio(RADIO_CS, 0, HW_SPI0); // Default radio used - Interrupt 0 because it's not used, 915 MHz default 



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
    InitSD();
    InitBMP();
    InitAccelGyro();
    InitMagnetometer();
    InitTempSensor();
    InitGPS();
    InitRadio();
}

SensorData* GetSensorData(){

    return &sdata;
}

void PrintSensorData(){ // loop

    PrintBMP();
    PrintAccelGyro();
    PrintMagnetometer();
    PrintTempSensor();
    PrintGPS();
    RWRadio();

    // BMP 
    BMP280Data* bmpdata = bmp280Sensor.read();

    // AccelGyro
    AccelGyroData* acceldata = accelGyro.read();

    // Magnetometer
    MagnetometerData* magdata = magnetometer.read();

    // Temperature
    float temperature = tempSensor.read();

    // GPS
    GPSData* gpsdata = gps.read();
    if (gps.hasFix()) {
        // Serial.print("Lat: "); Serial.println(gpsdata->latitude, 6);
        // Serial.print("Lon: "); Serial.println(gpsdata->longitude, 6);
        // Serial.print("Alt: "); Serial.println(gpsdata->altitude);
    } else {
        // Serial.println("No GPS fix available.");
    }

    // Read everything at same rate
    delay(1000);
}

void GetBarData(){

}

void InitBMP(){
    // Init BMP280
    if (bmp280Sensor.begin()) {
        Serial.println("BMP280 initialized.");
    } else {
        Serial.println("Initialization failed.");
    }
}

void InitAccelGyro() {
    // Init AccelGyro
    if (accelGyro.begin()) {
        Serial.println("AccelGyro initialized.");
    } else {
        Serial.println("Failed to initialize accelerometer/gyroscope sensor!");
    }
}

void InitMagnetometer() {
    // Init Magnetometer
    if (magnetometer.begin()) {
        Serial.println("Magnetometer initialized.");
    } else {
        Serial.println("Failed to initialize magnetometer!");
    }
}

void InitTempSensor(){
    // Init Temperature Sensor
    if (tempSensor.begin()) {
        Serial.println("Temperature sensor initialized.");
    } else {
        Serial.println("Failed to initialize temperature sensor!");
    }
}

void InitGPS() {
    gps.begin();
    gps.configure(1000, PMTK_SET_NMEA_OUTPUT_RMCONLY);
}

void InitRadio() {
    if (radio.begin()) {
        Serial.println("RFM95 initialized.");
    } else {
        Serial.println("Failed to initialize RFM95!");

    }
    radio.setFrequency(915.0);
    radio.setTxPower(20);
}

void PrintBMP(){
    BMP280Data* data = bmp280Sensor.read();
    Serial.print("Temp: ");
    Serial.print(data->temperature);
    Serial.print("°C, Pressure: ");
    Serial.print(data->pressure);
    Serial.print("Pa, Altitude: ");
    Serial.print(data->altitude);
    Serial.println("m");
    // delay(1000);
}

void PrintAccelGyro(){
    AccelGyroData* data = accelGyro.read();
    Serial.print("Accel: ");
    Serial.print(data->accelX);
    Serial.print(", ");
    Serial.print(data->accelY);
    Serial.print(", ");
    Serial.println(data->accelZ);

    Serial.print("Gyro: ");
    Serial.print(data->gyroX);
    Serial.print(", ");
    Serial.print(data->gyroY);
    Serial.print(", ");
    Serial.println(data->gyroZ);

    // delay(1000);

}

void PrintMagnetometer(){

    MagnetometerData* data = magnetometer.read();
    Serial.print("Magnetic Field: ");
    Serial.print(data->magneticX);
    Serial.print(", ");
    Serial.print(data->magneticY);
    Serial.print(", ");
    Serial.println(data->magneticZ);

    // delay(1000);
}

void PrintTempSensor(){
    float temperature = tempSensor.read();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    // delay(1000);
}

void PrintGPS() {
    GPSData* data = gps.read();
    if (gps.hasFix()) {
        Serial.print("Lat: "); Serial.println(data->latitude, 6);
        Serial.print("Lon: "); Serial.println(data->longitude, 6);
        Serial.print("Alt: "); Serial.println(data->altitude);
    } else {
        Serial.println("No GPS fix available.");
    }
    // delay(1000);
}

void RWRadio() {
    const char* message = "Hello, LoRa!";
    if (radio.send((const uint8_t*)message, strlen(message))) {
        Serial.println("Message sent!");
    }

    // Receive a message
    uint8_t buffer[64];
    size_t length;
    if (radio.receive(buffer, sizeof(buffer), length)) {
        Serial.print("Received: ");
        Serial.write(buffer, length);
        Serial.println();
    }

    // delay(1000);
}