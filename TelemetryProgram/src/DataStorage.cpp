#include <Arduino.h>
#include <DataStorage.h>

char *GetByteData(int len, int offset, char *output)
{
    Serial.printf("GetByteData()");
    return nullptr;
}

void InitDataStorage(){
    Serial.printf("InitDataStorage()");
}

void StoreBytes(char bytes[], int len){
    Serial.printf("StoreBytes()");
}