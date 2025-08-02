#pragma once

enum PacketType{
    PING = 1,
    ACK_PONG = 2,
    SENSOR_DATA = 3,
    COMMAND = 4
};

enum NetworkCommands{
    ENABLE_DEBUGGING = 1,
    FLIGHT_READY = 2,
    SWITCH_RADIO_FREQUENCY = 3
};