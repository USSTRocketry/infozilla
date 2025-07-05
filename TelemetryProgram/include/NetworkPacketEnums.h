enum PacketType{
    INVALID_EMPTY_PING = 1,
    PACKED_SENSOR_VALUES = 2,
    COMMAND = 3
};

enum NetworkCommands{
    ACK_PONG = 1,
    ENABLE_DEBUGGING = 2,
    FLIGHT_READY = 3,
    SWITCH_RADIO_FREQUENCY = 4
};