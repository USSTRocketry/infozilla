#pragma once

#include <SD.h>
#include <string>

void InitDataStorage();

void StoreBytes(char bytes[], int len);
void StoreStringLine(std::string s);

char* GetByteData(int len, int offset); // Return Bytes Array