// #include <Arduino.h>
// #include <DataPackager.h>
#include <fp16.h>
#include <SensorStructs.h>

char *convertedStruct;
int convertedStructLen = 0;

struct PackagedData
{
    short barval;
    short accelx;
    short accely;
    short accelz;
    short thermoval;
    short gyrox;
    short gyroy;
    short gyroz;
    long timestamp;
};

PackagedData LocalPackagedData;


void InitDataPackager(){
    //Serial.printf("InitDataPackager()");
    
}



// void HandleData() {
//     Serial.printf("HandleData()");
// }

// void TransferToSD(){
//     Serial.printf("TransferToSD()");
// }

uint16_t floatToHalf(float input){
    return fp16_ieee_from_fp32_value(input);
}

float halfToFloat(uint16_t input){
    return fp16_ieee_to_fp32_bits(input);
}

void convertStructToCompressedData(SensorData *data, PackagedData *packagedData){
    packagedData->barval = floatToHalf(data->barval);
    packagedData->accelx = floatToHalf(data->accelerometer.x);
    packagedData->accely = floatToHalf(data->accelerometer.y);
    packagedData->accelz = floatToHalf(data->accelerometer.z);
    packagedData->thermoval = floatToHalf(data->thermoval);
    packagedData->gyrox = floatToHalf(data->gyroscopeData.x);
    packagedData->gyroy = floatToHalf(data->gyroscopeData.y);
    packagedData->gyroz = floatToHalf(data->gyroscopeData.z);
    packagedData->timestamp = data->timestamp;
}


void update(){
    //convertStructToCompressedData(GetSensorData(), &LocalPackagedData);
}