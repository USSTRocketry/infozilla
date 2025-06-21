#pragma once

#include <SensorStructs.h>
#include <SD.h>
#include <string>

void InitDataStorage();

void StoreBytes(char bytes[], int len);
void StoreStringLine(std::string s);
std::string convertDataToCSVRow(SensorData* sensorData);

char* GetByteData(int len, int offset); // Return Bytes Array