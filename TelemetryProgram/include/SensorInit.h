#pragma once

bool InitBMP();
bool InitAccelGyro();
bool InitMagnetometer();
bool InitTempSensor();
void InitGPS();
bool InitRadio();

void PrintBMP();
void PrintAccelGyro();
void PrintMagnetometer();
void PrintTempSensor();
void PrintGPS();
void RWRadio();