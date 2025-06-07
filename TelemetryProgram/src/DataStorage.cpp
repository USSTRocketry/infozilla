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
    StoreStringLine("BarVal,Thermoval,accel_x,accell_y,accell_z,Gyro_x,Gyro_y,Gyro_z,timestamp");
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
    // BarVal,Thermoval,accel_x,accell_y,accell_z,Gyro_x,Gyro_y,Gyro_z,timestamp
    return std::to_string(sensorData->barval)+","+
    std::to_string(sensorData->thermoval)+","+std::to_string(sensorData->accelerometer.x)+","+
    std::to_string(sensorData->accelerometer.y)+","+std::to_string(sensorData->accelerometer.z)+","+
    std::to_string(sensorData->gyroscopeData.x)+","+std::to_string(sensorData->gyroscopeData.y)+","+
    std::to_string(sensorData->gyroscopeData.z)+","+std::to_string(sensorData->timestamp)+"\n";
}

void StoreData(SensorData* sensorData){
    StoreStringLine(convertDataToCSVRow(sensorData));
}