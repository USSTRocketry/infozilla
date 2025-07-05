#pragma once

#include <SensorStructs.h>
#include <DebugLights.h>

void GetBarData();
void PrintSensors();
void InitSensors(DebugLights debug_lights);

SensorData* GetSensorData();
void transmit_sensor_data(const SensorData& data);

// The gps needs to be sampled more frequently than the other sensors
// Sample less than once every 100 ms
void update_gps_data(void);

uint8_t * GetRadioByteData();

void SendRadioByteData(uint8_t *message, size_t length);