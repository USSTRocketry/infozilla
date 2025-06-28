#include <SD.h>
#include <SPI.h>
#include <string>

bool InitSD();

void WriteTest();

void StoreStringLineToCSV(std::string stringline);
