// #pragma once

// void InitDataPackager();

void HandleData();

typedef enum {
    PowerUp,
    ReadyForLaunch,
    InFlight,
    Landed,
    SaveData,
    PowerDown
}FlightStates;

FlightStates GetFlightState();

void SetFlightState(FlightStates flightState);

// void TransferToSD();