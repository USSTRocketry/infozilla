#include <Arduino.h>
// #include <DataPackager.h>
#include <fp16.h>
#include <SensorStructs.h>
#include <DataStorage.h>
#include <DataPackager.h>
#include <Sensors.h>
#include <SDHandler.h>
#include <string>

//this define is for the low power library needing this define but not having it defined.
#define voidFuncPtr void*
#include <ArduinoLowPower.h>
#include <FlexiTimer2.h>

// #include <ArduinoLowPower.h>
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



FlightStates FlightState = PowerUp;
double InitialHeightData;
double barvalQueue[20];
int BarvalStart = 0, BarvalEnd = 1;
uint32_t startTimeToSleep = 0;
bool isCollectingData = true;

//PackagedData LocalPackagedData;
char *packagesDataAsBytes;
void Update(float barval = 0);

void InitDataPackager(){
    //Serial.printf("InitDataPackager()");
    InitialHeightData = GetSensorData()->bmp280.altitude;
    
}

void AddValToQueue(double val){
    BarvalStart = (BarvalStart+1)%20;
    BarvalEnd = (BarvalEnd+1)%20;
    barvalQueue[BarvalStart] = val;
}

double ReadValFromQueue(){
    return barvalQueue[BarvalEnd];
}

void TransferToSD(){
    Serial.printf("TransferToSD()");
}

void HandleData() {

    if(isCollectingData){
        SensorData *RetrivedData = GetSensorData();

        Update(RetrivedData->bmp280.altitude);

        StoreBytes((char *)RetrivedData,sizeof(*RetrivedData));
    }else{
        Update();
    }

    //StoreBytes(GetSensorData(), sizeof(SensorData))
}

FlightStates GetFlightState(){
    return FlightState;
}

void SetFlightState(FlightStates flightState){
    FlightState = flightState;
}

void Update(float barval){
    
    double currentBarVal = barval;
    AddValToQueue(currentBarVal);
    switch (FlightState)
    {
    case PowerUp:
        /* dont do anything */
        isCollectingData = false;

        break;
    case ReadyForLaunch:
        isCollectingData = true;
        if(abs(InitialHeightData-currentBarVal) > 100.0){
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
        break;
    case PowerDown:
        // go to low power state 
        LowPower.deepSleep();
        // LowPower.deepSleep(5000);
        break;
    default:
        break;
    }
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
