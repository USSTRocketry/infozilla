#pragma once

void InitDataStorage();

void StoreBytes(int bytes[], int len);

int* GetByteData(int len, int offset); // Return Bytes Array