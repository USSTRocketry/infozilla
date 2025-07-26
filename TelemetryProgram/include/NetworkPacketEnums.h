#pragma once

enum PacketType{
    INVALID_EMPTY_PING = 1,
    PACKED_SENSOR_VALUES = 2,
    COMMAND = 3
};

enum NetworkCommands{
    PING = 1,
    ACK_PONG = 2,
    ENABLE_DEBUGGING = 3,
    FLIGHT_READY = 4,
    SWITCH_RADIO_FREQUENCY = 5
};