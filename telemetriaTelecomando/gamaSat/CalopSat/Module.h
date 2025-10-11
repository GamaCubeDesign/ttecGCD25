#ifndef MODULE_H
 #define MODULE_H
 
 #include <cstdint>
 #include "CommunicationProtocol.h"
 #include "Moden.h"
 #include "Timing.h"
 

 //extern SerialConsole console;
 extern uint8_t rxAddh;
 extern uint8_t rxAddl;
 extern uint8_t rxChan;
 extern uint8_t txAddh;
 extern uint8_t txAddl;
 extern uint8_t txChan;
 extern uint8_t rx_pointer;
 extern SatPacket satPacket;
 extern GSPacket gsPacket;
 extern uint8_t number_of_packets;
 extern const uint8_t N_status;
 extern const uint8_t N_imaging;
 void sendSatPacket();
 void updateRFComm();
 void onReceive();
 #endif