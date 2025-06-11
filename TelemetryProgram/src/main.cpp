// Setup code to run only once
#include <Arduino.h>
#include <Wire.h>
#include <SDHandler.h>
#include <SensorInit.h>
#include <Sensors.h>
#include <DataPackager.h>
#include <log.h>

// HAL libs
#include "Avionics_HAL.h"
#define PRINT_DEBUG 1
#define PRINT_FREQUENCY 1000 // Print sensor data every 1000 ms
#define TRANSMIT_FREQUENCY 500 // 500 ms
#define SENSOR_READ_FREQUENCY 100 // 100 ms

uint32_t currentMillis = 0;
uint32_t last_debug_time = 0;
uint32_t last_transmit_time = 0;
uint32_t last_heartbeat_time = 0;
uint32_t last_sensor_read_time = 0;
SensorData* sensorData;

bool led_on = false;



void print_sensor_data(SensorData* sensorData) {
    log_message("SENSOR", "----------------------------------------");
    log_message("SENSOR", "BMP280 - Temp: %.2f °C, Pressure: %.2f Pa, Altitude: %.2f m",
                sensorData->bmp280.temperature,
                sensorData->bmp280.pressure,
                sensorData->bmp280.altitude);

    log_message("SENSOR", "Accel: %.2f, %.2f, %.2f",
                sensorData->accelGyro.accelX,
                sensorData->accelGyro.accelY,
                sensorData->accelGyro.accelZ);

    log_message("SENSOR", "Gyro: %.2f, %.2f, %.2f",
                sensorData->accelGyro.gyroX,
                sensorData->accelGyro.gyroY,
                sensorData->accelGyro.gyroZ);

    log_message("SENSOR", "Magnetic Field: %.2f, %.2f, %.2f",
                sensorData->magnetometer.magneticX,
                sensorData->magnetometer.magneticY,
                sensorData->magnetometer.magneticZ);

    log_message("SENSOR", "GPS - Latitude: %.6f, Longitude: %.6f, Altitude: %.2f m, Speed: %.2f knots, Angle: %.2f degrees, Satellites: %d, Fix Quality: %d",
                sensorData->gps.latitude,
                sensorData->gps.longitude,
                sensorData->gps.altitude,
                sensorData->gps.speed,
                sensorData->gps.angle,
                sensorData->gps.satellites,
                sensorData->gps.fix_quality);

    log_message("SENSOR", "Temperature: %.2f °C", sensorData->temperature);

    log_message("SENSOR", "Timestamp: %lu", sensorData->timestamp);
    log_message("SENSOR", "----------------------------------------\n\n");
}

void setup() {

    // Heartbeat led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    led_on = true;

    Serial.begin(115200); // Setting this to 9600 somehow affects the GPS data reading, so we use 115200
    uint32_t ms = millis();
    while (!Serial && (millis() - ms < 3000)) {
        ; // wait for serial port to connect.
        // flash the LED to indicate waiting
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
    }

    InitSensors();
    InitSD();
    // WriteTest();
    log_message(__func__, "Sensors initialized.");
    
    currentMillis = millis();

}


void loop() {
    update_gps_data(); // Important!!!!
    currentMillis = millis();

    if(currentMillis - last_sensor_read_time >= SENSOR_READ_FREQUENCY) {
        sensorData = GetSensorData();
        last_sensor_read_time = currentMillis;

        //This saves and Handles the internal state machine.
        HandleData(sensorData);
    }

    if(PRINT_DEBUG && (currentMillis - last_debug_time >= PRINT_FREQUENCY)) {
        print_sensor_data(sensorData);
        last_debug_time = currentMillis;
    }

    if(currentMillis - last_transmit_time >= TRANSMIT_FREQUENCY){
        transmit_sensor_data(*sensorData);
        last_transmit_time = currentMillis;
    }

    // Heartbeat led
    if (currentMillis - last_heartbeat_time >= 1000) {
        if (led_on) {
            digitalWrite(LED_BUILTIN, LOW);
            led_on = false;
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
            led_on = true;
        }
        last_heartbeat_time = currentMillis;
    }
    
}



