#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <Arduino.h>

enum Protocol : uint8_t {
    HEALTH_PROTOCOL = 0,
    AIS_PROTOCOL = 1,
    MEC_PROTOCOL = 2,
    IMAGING_PROTOCOL = 3,
    CONTROL_PROTOCOL = 4,
    TERM_PROTOCOL = 5,
};

enum Operation : uint8_t {
    GENERATE_HEALTH_DATA = 0,
    RESEND_HEALTH_DATA = 1,
};


struct SatPacket{
    uint8_t length;
    Protocol protocol;
    Operation operation;
};

struct GSPacket{
    uint8_t length;
    Protocol protocol;
    Operation operation;
};

struct healthData{
    float voltage;
    float current;
    float power;
    float shunt;
    float temperature;
    float pressure;
    float altitude;
    float temperatureD;
    unsigned long int sd_memory_usage;
    uint8_t length;
};

void sendPacket(Protocol protocol, Operation operation);


#endif