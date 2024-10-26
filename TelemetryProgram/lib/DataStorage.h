#pragma once

void InitDataStorage();

void StoreBytes(char bytes[], int len);

char* GetByteData(int len, int offset); // Return Bytes Array