#include "Arduino.h"
#include "GroundStationComm.h"
#include "CommunicationProtocol.h"


GSPacket gspacket;


void menu(){
    Serial.println("\n                   Available commands:");
    Serial.println("    PROTOCOL     |      OPERATION           |      COMAND");
    Serial.println("STATUS_PROTOCOL  : INITCOMM                 ->     initcomm");
    Serial.println("STATUS_PROTOCOL  : VERIFY_FILE              ->     vfile");
    Serial.println("STATUS_PROTOCOL  : VERIFY_INFO              ->     vinfo");
    Serial.println("HEALTH_PROTOCOL  : GENERATE_HEALTH_DATA     ->     ghdata");
    Serial.println("HEALTH_PROTOCOL  : RESEND_HEALTH_DATA       ->     rhdata");
    Serial.println("CONTROL_PROTOCOL : SOLAR_VECTOR             ->     solarvector");
    Serial.println("CONTROL_PROTOCOL : TWO_VECTORS              ->     2vectors");
    Serial.println("CONTROL_PROTOCOL : SUN_POINTING             ->     sunpointing");
    Serial.println("CONTROL_PROTOCOL : STABILIZATION            ->     stabili");

    
    
    
    
    Serial.println();
}
/*
void printFIFO(bool HealthFIFO, bool ImagingFIFO, int HealthCells, int ImagingCells){
    Serial.println("HEALTH FIFO            |           IMAGING FIFO");
    Serial.print("FIFO enabled: ")
    Serial.print(HelthFIFO)
    Serial.print(ImagingFIFO)
    Serial.print("Number of useful cells: ")
    Serial.print(HealthCells);
    Serial.print(ImagingCells);
}
*/
