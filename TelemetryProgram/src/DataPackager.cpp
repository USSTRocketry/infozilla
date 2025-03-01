#include <Arduino.h>
// #include <DataPackager.h>
#include <fp16.h>
#include <SensorStructs.h>
#include <DataStorage.h>
#include <Sensors.h>
#include <SDHandler.h>
#include <string>

// char *convertedStruct;
// int convertedStructLen = 0;

// struct PackagedData
// {
//     short barval;//2
//     short accelx;//2
//     short accely;//2
//     short accelz;//2
//     short thermoval;//2
//     short gyrox;//2
//     short gyroy;//2
//     short gyroz;//2
//     long timestamp;//8?
// };

//PackagedData LocalPackagedData;
char *packagesDataAsBytes;

void InitDataPackager(){
    //Serial.printf("InitDataPackager()");
    
}

std::string ConvertSensorDataToString(SensorData data){
    std::string s = std::to_string(data.barval)+","+
    std::to_string(data.thermoval)+","
    +std::to_string(data.accelerometer.x)+","
    +std::to_string(data.accelerometer.y)+","
    +std::to_string(data.accelerometer.z)+","
    +std::to_string(data.gyroscopeData.x)+","
    +std::to_string(data.gyroscopeData.y)+","
    +std::to_string(data.gyroscopeData.z)+","
    +std::to_string(data.timestamp)+",";

    return s;
}

void HandleData() {
    Serial.printf("HandleData()");

    //StoreBytes(GetSensorData(), sizeof(SensorData))
}

void TransferToSD(){
    Serial.printf("TransferToSD()");
}

// uint16_t floatToHalf(float input){
//     return fp16_ieee_from_fp32_value(input);
// }

// float halfToFloat(uint16_t input){
//     return fp16_ieee_to_fp32_bits(input);
// }

// void convertStructToCompressedData(SensorData *data, PackagedData *packagedData){
//     packagedData->barval = floatToHalf(data->barval);
//     packagedData->accelx = floatToHalf(data->accelerometer.x);
//     packagedData->accely = floatToHalf(data->accelerometer.y);
//     packagedData->accelz = floatToHalf(data->accelerometer.z);
//     packagedData->thermoval = floatToHalf(data->thermoval);
//     packagedData->gyrox = floatToHalf(data->gyroscopeData.x);
//     packagedData->gyroy = floatToHalf(data->gyroscopeData.y);
//     packagedData->gyroz = floatToHalf(data->gyroscopeData.z);
//     packagedData->timestamp = data->timestamp;
// }
