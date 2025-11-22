#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <Arduino.h>

enum Protocol : int {
    HEALTH_PROTOCOL = 0,
    AIS_PROTOCOL = 1,
    MEC_PROTOCOL = 2,
    IMAGING_PROTOCOL = 3,
    CONTROL_PROTOCOL = 4,
    TERM_PROTOCOL = 5,
    STATUS_PROTOCOL = 6,

};

enum Operation : int {
    //health data
    GENERATE_HEALTH_DATA = 0,
    RESEND_HEALTH_DATA = 1,

    //status data
    INITCOMM = 2,
    VERIFY_FILE = 3,
    SHUT_DOWN_SYSTEM = 4,

    //Mechanical control
    OPEN_ANTENNAS = 5,

    //Termal control
    ACTIVATE_THERMAL_CONTROL = 6,
    DEACTIVATE_THERMAL_CONTROL = 7,

    //Control
    SOLAR_VECTOR = 8,
    TWO_VECTORS = 9,
    SUN_POINTING = 10,
    STABILIZATION = 11,
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
    int vector1;
    int vector2;
};

struct control{
    uint8_t length;
    Protocol protocol;
    Operation operation;
    int vector1;
    int vector2;
    int solarVector;   
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

extern GSPacket packet;
extern SatPacket resposta;
void sendPacket(Protocol protocol, Operation operation);
void onReceive();
bool receivePacket(SatPacket *packet, unsigned long timeout_ms);
void sendVectors();
void clearSerial();

#endif