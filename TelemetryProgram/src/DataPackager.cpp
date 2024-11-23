#include <Arduino.h>
#include <DataPackager.h>

void InitDataPackager(){
    Serial.printf("InitDataPackager()");
}

void HandleData() {
    Serial.printf("HandleData()");
}

void TransferToSD(){
    Serial.printf("TransferToSD()");
}