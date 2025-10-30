#include "Arduino.h"
#include "GroundStationComm.h"
#include "CommunicationProtocol.h"

healthData health;

GSPacket gspacket;


void menu(){
    Serial.println("Available commands:");
    Serial.println("HEALTH_PROTOCOL : GENERATE_HEALTH_DATA  ");
    Serial.println("HEALTH_PROTOCOL : RESEND_HEALTH_DATA  ");
    Serial.println("STATUS_PROTOCOL : INITCOMM  ");
    Serial.println("STATUS_PROTOCOL : VERIFY_FILE  ");
    Serial.println();
}


