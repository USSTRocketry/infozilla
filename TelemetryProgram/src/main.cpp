
#include <Arduino.h>


#include "SensorStructs.h"
#include "DataPackager.h"
#include "DataStorage.h"
#include "GPS.h"
#include "Radio.h"
#include "SDHandler.h"
#include "Sensors.h"

char data[sizeof(SensorData)];

int i = 0;
// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(0);
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  i++;
  Serial.printf("Hello World: %i\n", i);
  delay(500);
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void Handledata(){

}

void TransferDataToSD(){

}

char *TurnSensorDataToBytes(SensorData *data){
  return NULL;

}