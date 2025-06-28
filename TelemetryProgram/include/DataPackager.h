void Update(float barval = 0);
void HandleData(SensorData *RetrivedData);

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
