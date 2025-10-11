#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H
#include <cstdint>   // adiciona no topo do arquivo

struct SatPacket{
    uint8_t length;
    char txt[32];
};

struct GSPacket{
    uint8_t length;
    char txt[32];
};

extern SatPacket satPacket;
extern GSPacket gsPacket;
#endif