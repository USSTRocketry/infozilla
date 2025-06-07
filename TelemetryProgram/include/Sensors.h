#pragma once

#include <SensorStructs.h>

void GetBarData();
void PrintSensors();
void InitSensors();

SensorData* GetSensorData();
void transmit_sensor_data(const SensorData& data);

// The gps needs to be sampled more frequently than the other sensors
// Sample less than once every 100 ms
void update_gps_data(void);