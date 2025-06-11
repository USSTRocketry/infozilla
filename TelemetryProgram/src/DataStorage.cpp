// #include <Arduino.h>
#include <DataStorage.h>
#include <SensorStructs.h>
#include <string>

#include <SerialFlash.h>
#include <SD.h>
#include <SPI.h>

const int SDchipSelect = 4;    // Audio Shield has SD card CS on pin 10
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
//const int FlashChipSelect = 21; // Arduino 101 built-in SPI Flash

void InitSdCard(){
    if (!SD.begin(SDchipSelect)) {
    //error("Unable to access SD card");
    }
    if (!SerialFlash.begin(FlashChipSelect)) {
    //error("Unable to access SPI Flash chip");
    }

    //File rootdir = SD.open("/");
    if(SD.exists("/FlightData.fdat")){
        SD.remove("/FlightData.fdat");
    }

    SerialFlash.create("/FlightData.fdat", sizeof("/FlightData.fdat")); 
    File file = SD.open("/FlightData.fdat",O_CREAT & O_WRITE);

    file.write("BarVal,Thermoval,accel_x,accell_y,accell_z,Gyro_x,Gyro_y,Gyro_z,timestamp\n");

    file.close();

}

// char *GetByteData(int len, int offset, char *output)
// {
//     Serial.printf("GetByteData()");
//     return nullptr;
// }

void InitDataStorage(){
    Serial.printf("InitDataStorage()");
    StoreStringLine("Altitude,Pressure,Temperature,Acceleration_X,Acceleration_Y,Acceleration_Z,gyroX,gyroY,gyroZ,\
    magneticX,magneticY,magneticX,GPS_Altitude,GPS_Angle,GPS_Fix_Quality,GPS_Latitude,GPS_Longitude,GPS_Speed,Timestamp");
}

// void StoreBytes(char bytes[], int len){
//     Serial.printf("StoreBytes()");
// }

void StoreStringLine(std::string s){
    File file = SD.open("/FlightData.fdat",O_APPEND);

    //this should be the maximum size of the input string;
    char buffer[256];

    strcpy(buffer,s.c_str());

    file.write(buffer);

    file.close();
}

void TransferFileData(File to){
    File from = SD.open("/FlightData.fdat",O_READ);
    unsigned long count = from.size();
    while (count > 0) {
        char buff[128];
        unsigned long n = count;
        if (n > 128) n = 128;
        from.read(buff, n);
        to.write(buff, n);
        count = count - n;
    }
    from.close();
}

std::string convertDataToCSVRow(SensorData* sensorData){

    // BMP280Data bmp280;
    // AccelGyroData accelGyro;
    // MagnetometerData magnetometer;
    // GPSData gps;
    // float temperature;
    // long int timestamp;

    // BarVal,Thermoval,accel_x,accell_y,accell_z,Gyro_x,Gyro_y,Gyro_z,timestamp
    // Altitude,Pressure,Temperature,Acceleration_X,Acceleration_Y,Acceleration_Z,gyroX,gyroY,gyroZ,
    // magneticX,magneticY,magneticX,GPS_Altitude,GPS_Angle,GPS_Fix_Quality,GPS_Latitude,GPS_Longitude,GPS_Speed,Timestamp
    // TODO: Some data is missing here, like GPS data.

    
    return std::to_string(sensorData->bmp280.altitude) + "," +
            std::to_string(sensorData->bmp280.pressure) + "," +

           std::to_string(sensorData->temperature) + "," +

           std::to_string(sensorData->accelGyro.accelX) + "," +
           std::to_string(sensorData->accelGyro.accelY) + "," +
           std::to_string(sensorData->accelGyro.accelZ) + "," +

            std::to_string(sensorData->accelGyro.gyroX) + "," +
            std::to_string(sensorData->accelGyro.gyroY) + "," +
            std::to_string(sensorData->accelGyro.gyroZ) + "," +

           std::to_string(sensorData->magnetometer.magneticX) + "," +
           std::to_string(sensorData->magnetometer.magneticY) + "," +
           std::to_string(sensorData->magnetometer.magneticZ) + "," +

            std::to_string(sensorData->gps.altitude) + "," +
            std::to_string(sensorData->gps.angle) + "," +
            std::to_string(sensorData->gps.fix_quality) + "," +
            std::to_string(sensorData->gps.latitude) + "," +
            std::to_string(sensorData->gps.longitude) + "," +
            std::to_string(sensorData->gps.speed) + "," +

           std::to_string(sensorData->timestamp) + "\n";
}

void StoreData(SensorData* sensorData){
    StoreStringLine(convertDataToCSVRow(sensorData));
}