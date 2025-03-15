#include <Arduino.h>
// #include <DataPackager.h>
#include <fp16.h>
#include <SensorStructs.h>
#include <DataStorage.h>
#include <Sensors.h>
#include <SDHandler.h>
#include <string>
#include <ArduinoLowPower.h>
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

typedef enum {
    PowerUp,
    InFlight,
    Landed,
    SaveData,
    PowerDown
}FlightStates;

FlightStates FlightState = PowerUp;
double InitialHeightData;
double barvalQueue[20];
int BarvalStart = 0, BarvalEvd = 1;

//PackagedData LocalPackagedData;
char *packagesDataAsBytes;

void InitDataPackager(){
    //Serial.printf("InitDataPackager()");
    InitialHeightData = GetSensorData()->barval;
    
}

std::string ConvertSensorDataToString(SensorData* data){
    std::string s = std::to_string(data->barval)+","+
    std::to_string(data->thermoval)+","
    +std::to_string(data->accelerometer.x)+","
    +std::to_string(data->accelerometer.y)+","
    +std::to_string(data->accelerometer.z)+","
    +std::to_string(data->gyroscopeData.x)+","
    +std::to_string(data->gyroscopeData.y)+","
    +std::to_string(data->gyroscopeData.z)+","
    +std::to_string(data->timestamp)+",";

    return s;
}

void HandleData() {
    Serial.printf("HandleData()");

    //StoreBytes(GetSensorData(), sizeof(SensorData))
}

void Update(){
    SensorData *RetrivedData = GetSensorData();
    double currentBarVal = RetrivedData->barval;
    AddValToQueue(currentBarVal);
    switch (FlightState)
    {
    case PowerUp:
        /* code */
        if(abs(InitialHeightData-currentBarVal) > 100.0){
            FlightState = InFlight;
        }

        break;
    case InFlight:
        if(abs(ReadValFromQueue()-currentBarVal) < 1.0){
            FlightState = Landed;
        }
        break;
    case Landed:
        // todo: Wait about 60 seconds
        break;
    case SaveData:
        // todo: Disable Data collection and move all files from builtin flash to sd
        break;
    case PowerDown:
        // todo: go to low power state 
        LowPower.deepSleep(5000);
        break;
    default:
        break;
    }
}

void AddValToQueue(double val){
    BarvalStart = (BarvalStart+1)%20;
    BarvalEvd = (BarvalEvd+1)%20;
    barvalQueue[BarvalStart] = val;
}

double ReadValFromQueue(){
    return barvalQueue[BarvalEvd];
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
