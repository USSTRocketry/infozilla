#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <SensorStructs.h>
#include <SensorInit.h>
#include <SDHandler.h>
#include <log.h>
#include <Sensors.h>


// HAL libs
#include "Avionics_HAL.h"

DebugLights debug_lights;

SensorData sdata;

// Constructors for sensors
SensorBMP280 bmp280Sensor; // SensorBMP280 - Temp, pressure, and altitude
SensorAccelGyro accelGyro(0x6B, I2C_WIRE0); // Accelerometer and gyroscope sensor Default I2C address and bus
SensorMagnetometer magnetometer(0x1E, I2C_WIRE0); // Magnometer Default I2C address and bus
SensorTemperature tempSensor; // Default I2C address and bus
GPS gps(GPS_HW_SERIAL, 9600);
// RYLR998Radio radio(HW_SERIAL8, 9600); // Not default Radio to be used
const float RADIO_FREQUENCY = 915.0; // Default radio frequency
RFM95Radio radio(RADIO_CS, RADIO_INT, RADIO_SPI, RADIO_FREQUENCY); // Default radio used - Interrupt 0 because it's not used, 915 MHz default 



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

void InitSensors(DebugLights debug_lights_in){
    debug_lights = debug_lights_in;
    log_message(__func__, "Initializing sensors...");
    if(!InitSD()){
        debug_lights.AddError(INIT_SD_ERR);
    }
    InitBMP();
    InitAccelGyro();
    InitMagnetometer();
    InitTempSensor();
    InitGPS();
    InitRadio();
}

SensorData* GetSensorData(){

    BMP280Data* bmpData = bmp280Sensor.read();
    sdata.bmp280.temperature = bmpData->temperature;
    sdata.bmp280.pressure = bmpData->pressure;
    sdata.bmp280.altitude = bmpData->altitude;


    AccelGyroData* accelData = accelGyro.read();
    sdata.accelGyro.temperature = accelData->temperature;
    sdata.accelGyro.accelX = accelData->accelX;
    sdata.accelGyro.accelY = accelData->accelY;
    sdata.accelGyro.accelZ = accelData->accelZ;
    sdata.accelGyro.gyroX = accelData->gyroX;
    sdata.accelGyro.gyroY = accelData->gyroY;
    sdata.accelGyro.gyroZ = accelData->gyroZ;


    MagnetometerData* magData = magnetometer.read();
    sdata.magnetometer.magneticX = magData->magneticX;
    sdata.magnetometer.magneticY = magData->magneticY;
    sdata.magnetometer.magneticZ = magData->magneticZ;

    float temperature = tempSensor.read();
    sdata.temperature = temperature;

    GPSData* gpsData = gps.read();
    if (gpsData != nullptr){
        sdata.gps.latitude = gpsData->latitude;
        sdata.gps.longitude = gpsData->longitude;
        sdata.gps.altitude = gpsData->altitude;
        sdata.gps.speed = gpsData->speed;
        sdata.gps.angle = gpsData->angle;
        sdata.gps.satellites = gpsData->satellites;
        sdata.gps.fix_quality = gpsData->fix_quality;
    }else{
        log_message(__func__, "GPS data is null, setting to 0s.");
        sdata.gps.latitude = 0.0f;
        sdata.gps.longitude = 0.0f;
        sdata.gps.altitude = 0.0f;
        sdata.gps.speed = 0.0f;
        sdata.gps.angle = 0.0f;
        sdata.gps.satellites = 0;
        sdata.gps.fix_quality = 0;
    }
    

    sdata.timestamp = millis();

    return &sdata;
}

void update_gps_data(void){
    // Update GPS data
    gps.update();
}

void PrintSensors(){ // loop

    PrintBMP();
    PrintAccelGyro();
    PrintMagnetometer();
    PrintTempSensor();
    PrintGPS();
}

bool InitBMP(){
    // Init BMP280
    if (bmp280Sensor.begin()) {
        log_message(__func__, "BMP280 initialized.");
        return true;
    } else {
        log_message(__func__, "Failed to initialize BMP280 sensor!");
        debug_lights.AddError(INIT_BMP_ERR);
        return false;
    }
}

bool InitAccelGyro() {
    // Init AccelGyro
    if (accelGyro.begin()) {
        
        log_message(__func__, "AccelGyro initialized.");
        return true;
    } else {
        log_message(__func__, "Failed to initialize accelerometer/gyroscope sensor!");
        debug_lights.AddError(INIT_ACCELGYRO_ERR);
        return false;
    }
}

bool InitMagnetometer() {
    // Init Magnetometer
    if (magnetometer.begin()) {
        return true;
        log_message(__func__, "Magnetometer initialized.");
    } else {
        log_message(__func__, "Failed to initialize magnetometer!");
        debug_lights.AddError(INIT_MAGNETOMETER_ERR);
        return false;
    }
}

bool InitTempSensor(){
    // Init Temperature Sensor
    if (tempSensor.begin()) {
        return true;
        log_message(__func__, "Temperature sensor initialized.");
    } else {
        log_message(__func__, "Failed to initialize temperature sensor!");
        debug_lights.AddError(INIT_THERMOMETER_ERR);
        return false;
    }
}

void InitGPS() {
    if(gps.begin()){
        log_message(__func__, "GPS initialized.");
    }else{
        log_message(__func__, "GPS Failed to initialize.");
        debug_lights.AddError(INIT_GPS_ERR);
    }
    
    
}

bool InitRadio() {
    bool successs = radio.begin();
    if (successs) {
        log_message(__func__, "RFM95 initialized.");
    } else {
        log_message(__func__, "Failed to initialize RFM95!");
        debug_lights.AddError(INIT_RADIO_ERR);
    }
    radio.setFrequency(RADIO_FREQUENCY);
    radio.setTxPower(20);

    return successs;
}

void PrintBMP(){
    BMP280Data* data = bmp280Sensor.read();
    log_message(__func__, "Temp: %.2f°C, Pressure: %.2fPa, Altitude: %.2fm",
                data->temperature, data->pressure, data->altitude);
}

void PrintAccelGyro(){
    AccelGyroData* data = accelGyro.read();
    log_message(__func__, "Accel: %.2f, %.2f, %.2f", data->accelX, data->accelY, data->accelZ);
    log_message(__func__, "Gyro: %.2f, %.2f, %.2f", data->gyroX, data->gyroY, data->gyroZ);
}

void PrintMagnetometer(){

    MagnetometerData* data = magnetometer.read();
    log_message(__func__, "Magnetic Field: %.2f, %.2f, %.2f", data->magneticX, data->magneticY, data->magneticZ);
}

void PrintTempSensor(){
    float temperature = tempSensor.read();
    log_message(__func__, "Temperature: %.2f °C", temperature);
}

void PrintGPS() {
    GPSData* data = gps.read();
    if (gps.hasFix()) {
        log_message(__func__, "Lat: %.6f", data->latitude);
        log_message(__func__, "Lon: %.6f", data->longitude);
        log_message(__func__, "Alt: %.2f", data->altitude);
    } else {
        log_message(__func__, "No GPS fix available.");
    }
}

void RWRadio() {
    const char* message = "Hello, LoRa!";
    Serial.println("Radio Started.");
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
    Serial.println("Radio Finished.");
}

uint8_t RadioBuffer[256];

std::pair<u_int8_t*, size_t> GetRadioByteData(){
    size_t msgLength;
    bool status = radio.receive(RadioBuffer,256,msgLength);
    if (!status || msgLength == 0) {
        return std::make_pair(nullptr, 0);
    }
    return std::make_pair(RadioBuffer, msgLength);
}

void SendRadioByteData(uint8_t *message, size_t length){
    radio.send(message,length);
}


// Radio packaging
/*
"""
+--------------------+-----------+-------+------------+-------------------------------+
| Field              | Type      | Bytes | Multiplier | Description                   |
+--------------------+-----------+-------+------------+-------------------------------+
| BMP280 Temp        | int16_t   | 2     | *100       | Temperature in °C             |
| Pressure           | uint32_t  | 4     | *100       | Pressure in hPa               |
| BMP280 Altitude    | int16_t   | 2     | *10        | Altitude in meters            |
| Accel X/Y/Z        | int16_t*3 | 6     | *100       | Acceleration in m/s²          |
| Gyro X/Y/Z         | int16_t*3 | 6     | *100       | Angular velocity in °/s       |
| IMU Temp           | int16_t   | 2     | *100       | IMU temperature in °C         |
| Mag X/Y/Z          | int16_t*3 | 6     | *100       | Magnetic field in µT          |
| Extra Temp Sensor  | int16_t   | 2     | *100       | External temp in °C           |
| GPS Latitude       | int32_t   | 4     | *1e7       | Degrees                       |
| GPS Longitude      | int32_t   | 4     | *1e7       | Degrees                       |
| GPS Altitude       | int16_t   | 2     | *10        | Altitude in meters            |
| GPS Speed          | uint16_t  | 2     | *100       | Speed in knots                |
| GPS Angle          | uint16_t  | 2     | *100       | Heading angle in degrees      |
| timestamp          | uint32_t  | 4     | *1         | Timestamp of data             |
+--------------------+-----------+-------+------------+-------------------------------+
| TOTAL              |           | 54    |            |                               |
+--------------------+-----------+-------+------------+-------------------------------+

"""

# Define the format string for struct.unpack
FORMAT = "<h I h 3h 3h h 3h h i i h H H I"
*/


#pragma pack(push, 1)  // Ensure no padding is added between struct members
struct PacketData {
    int16_t bmp280_temp;        // Temperature in °C (*100)
    uint32_t pressure;          // Pressure in hPa (*100)
    int16_t bmp280_altitude;    // Altitude in meters (*10)
    int16_t accel_x;            // Acceleration X in m/s² (*100)
    int16_t accel_y;            // Acceleration Y in m/s² (*100)
    int16_t accel_z;            // Acceleration Z in m/s² (*100)
    int16_t gyro_x;             // Angular velocity X in °/s (*100)
    int16_t gyro_y;             // Angular velocity Y in °/s (*100)
    int16_t gyro_z;             // Angular velocity Z in °/s (*100)
    int16_t imu_temp;           // IMU temperature in °C (*100)
    int16_t mag_x;              // Magnetic field X in µT (*100)
    int16_t mag_y;              // Magnetic field Y in µT (*100)
    int16_t mag_z;              // Magnetic field Z in µT (*100)
    int16_t extra_temp_sensor;  // Extra temp in °C (*100)
    int32_t gps_latitude;       // Latitude in degrees (*1e7)
    int32_t gps_longitude;      // Longitude in degrees (*1e7)
    int16_t gps_altitude;       // Altitude in meters (*10)
    uint16_t gps_speed;         // Speed in knots (*100)
    uint16_t gps_angle;         // Heading angle in degrees (*100)
    uint32_t timestamp;         // Timestamp of data
};
#pragma pack(pop)

void sensor_data_to_radio_packet(const SensorData& data, struct PacketData& packetData) {
    packetData.bmp280_temp = static_cast<int16_t>(data.bmp280.temperature * 100);
    packetData.pressure = static_cast<uint32_t>(data.bmp280.pressure * 100);
    packetData.bmp280_altitude = static_cast<int16_t>(data.bmp280.altitude * 10);
    packetData.accel_x = static_cast<int16_t>(data.accelGyro.accelX * 100);
    packetData.accel_y = static_cast<int16_t>(data.accelGyro.accelY * 100);
    packetData.accel_z = static_cast<int16_t>(data.accelGyro.accelZ * 100);
    packetData.gyro_x = static_cast<int16_t>(data.accelGyro.gyroX * 100);
    packetData.gyro_y = static_cast<int16_t>(data.accelGyro.gyroY * 100);
    packetData.gyro_z = static_cast<int16_t>(data.accelGyro.gyroZ * 100);
    packetData.imu_temp = static_cast<int16_t>(data.accelGyro.temperature* 100);
    packetData.mag_x = static_cast<int16_t>(data.magnetometer.magneticX * 100);
    packetData.mag_y = static_cast<int16_t>(data.magnetometer.magneticY * 100);
    packetData.mag_z = static_cast<int16_t>(data.magnetometer.magneticZ * 100);
    packetData.extra_temp_sensor = static_cast<int16_t>(data.temperature * 100);
    packetData.gps_latitude = static_cast<int32_t>(data.gps.latitude * 1e7);
    packetData.gps_longitude = static_cast<int32_t>(data.gps.longitude * 1e7);
    packetData.gps_altitude = static_cast<int16_t>(data.gps.altitude * 10);
    packetData.gps_speed = static_cast<uint16_t>(data.gps.speed * 100);
    packetData.gps_angle = static_cast<uint16_t>(data.gps.angle * 100);
    packetData.timestamp = data.timestamp;
}

void transmit_sensor_data(const SensorData& data) {
    struct PacketData packetData;
    sensor_data_to_radio_packet(data, packetData);

    // Send the packet over the radio
    if (radio.send((const uint8_t*)&packetData, sizeof(packetData))) {
        log_message(__func__, "Sensor data transmitted successfully.");
    } else {
        log_message(__func__, "Failed to transmit sensor data.");
    }
}


/**
 *  Switch transmission frequency of the radio to one specified by the ground station.
 *  Frequency switch routine:
 *      Center (ground station): "switch radio frequency X"
 *      Peripheral: "echo switch radio frequency X"
 *      Center: "Ack"
 *      ===== peripheral and center actually switches frequency here ======
 *    Note: There should be a timeout of 3 seconds at each step and if reached, the routine is aborted
 *        and the radio frequency is not changed.
 * @param message The message received from the ground station.
 *                 Byte 0 = COMMAND
 *                 Byte 1 = SWITCH_RADIO_FREQUENCY
 *                 Bytes 2-5 (inclusive) = Frequency in MHz (as float)
 * @param length The length of the message.
 */
void SwitchRadioFrequency(uint8_t* message, size_t length) {
    if (length < 6 || message[0] != COMMAND || message[1] != SWITCH_RADIO_FREQUENCY) {
        log_message(__func__, "Invalid message format for frequency switch.");
        return;
    }

    // Extract the frequency from the message
    float newFrequency = *((float*)&message[2]);
    log_message(__func__, "Initiating procedure to switch frequency to: %.2f MHz", newFrequency);

    // Acknowledge the command
    uint8_t ackMessage[7] = {COMMAND, ACK_PONG, SWITCH_RADIO_FREQUENCY};
    *((float*)&ackMessage[3]) = newFrequency; // Include the new frequency in the ack message
    SendRadioByteData(ackMessage, sizeof(ackMessage));

    // Wait for the acknowledgment from the ground station
    uint32_t cur_time = millis();
    std::pair<u_int8_t*, size_t> res = GetRadioByteData();
    bool ackReceived = false;
    while (millis() - cur_time < 3000) {
        // Wait for a maximum of 3 seconds for the acknowledgment
        if (res.first != nullptr && res.second > 0 
            && res.first[0] == COMMAND 
            && res.first[1] == ACK_PONG 
            && res.first[2] == SWITCH_RADIO_FREQUENCY
        ) {
            ackReceived = true;
            break;
        }
        delay(100); // Polling delay
        res = GetRadioByteData();
    }
    
    if (!ackReceived) {
        log_message(__func__, "No acknowledgment received for frequency switch.");
        return; // Abort if no acknowledgment is received
    }

    // Set the new frequency
    radio.setFrequency(newFrequency);
    log_message(__func__, "Radio frequency switched to %.2f MHz", newFrequency);
}