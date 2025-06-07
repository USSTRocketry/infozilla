
#pragma once
#include <Arduino.h>

static inline void log_print(const char* message) {
    // Serial will not be available when powered by the battery through the gpio pins
    if(Serial){
        Serial.print(message);
    }
}

// int overloaded function to print a number
static inline void log_print(int number) {
    if(Serial){
        Serial.print(number);
    }
}
// float overloaded function to print a number
static inline void log_print(float number) {
    if(Serial){
        Serial.print(number);
    }
}
// double overloaded function to print a number
static inline void log_print(double number) {
    if(Serial){
        Serial.print(number);
    }
}
// long int overloaded function to print a number
static inline void log_print(long int number) {
    if(Serial){
        Serial.print(number);
    }
}

static inline void log_println(const char* message) {
    if(Serial){
        Serial.println(message);
    }
}

static inline void log_message(const char* app, const char* fmt, ...) {
    char message[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    if(Serial){
        Serial.print("[ ");
        Serial.print(millis());
        Serial.print(" ] ");
        Serial.print(app);
        Serial.print(": ");
        Serial.print(message);
        Serial.println("");
    }
}