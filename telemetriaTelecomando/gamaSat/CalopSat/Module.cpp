#include "Module.h"
#include "Moden.h"
#include "Integration.h"
#include <iostream>


bool talking = false;


unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;

SatPacket satPacket;
GSPacket gsPacket;
HealthStatus hStatus;
control controlD;
uint8_t rx_pointer = 0;

void sendSatPacket() {
    tx_send((uint8_t*)&satPacket, satPacket.length);
    std::cout << "Protocol: " << satPacket.protocol << "\n";
    std::cout << "Operation: " << satPacket.operation << "\n";
}

void sendHealthStatus() {
    tx_send((uint8_t*)&hStatus, hStatus.length);
    std::cout << "Protocol: " << hStatus.protocol << "\n";
    std::cout << "Operation: " << hStatus.operation << "\n";
    std::cout << "Number of Packages: " << (int)hStatus.numberOfPackages << "\n";
}

void updateRFComm() {
    std::cout << "\n Waiting for packet. \n";

    uint8_t b;
    if (modemAvailable()) {
        while (modemAvailable()) {
            b = modemRead();
            ((uint8_t*)(&gsPacket))[rx_pointer++] = b; //transforma a struct em array de bytes e vai colocando cada byte no seu lugar.

            if (rx_pointer > 0 && rx_pointer == gsPacket.length) {
                std::cout << "Packet received from the ground station: ";
                for (int i = 0; i < gsPacket.length; i++) {
                    std::cout << (int)((uint8_t*)&gsPacket)[i] << " ";
                }
                std::cout << std::endl;
                onReceive();
                rx_pointer = 0;
            }

            if (rx_pointer > sizeof(gsPacket)) {
                rx_pointer = 0;
            }
        }

        //communication_timeout = millis() + communication_timeout_limit;
    }
}


//Essa função decide o que fazer de acordo com o comando recebido
void onReceive() {
    std::cout << "Protocol processing:" << std::endl;
    std::cout << "\nProtocol:" << (int)gsPacket.protocol << std::endl;
    std::cout << "\nOperation:" << (int)gsPacket.operation << std::endl;
    
    switch (gsPacket.protocol) {
        case HEALTH_PROTOCOL:
            std::cout << "\nHEALTHPROTOCOL\n" << std::endl;
            switchHealthProtocol();
            break;
        case AIS_PROTOCOL:
            std::cout << "\nAISPROTOCOL\n" << std::endl;
            switchAISProtocol();
            break;
        case MEC_PROTOCOL:
            std::cout << "\nMECPROTOCOL\n" << std::endl;
            switchMECProtocol();
            break;
        case IMAGING_PROTOCOL:
            std::cout << "\nIMAGINGPROTOCOL\n" << std::endl;
            switchImagingProtocol();
            break;
        case CONTROL_PROTOCOL:
            std::cout << "\nCONTROLPROTOCOL\n" << std::endl;
            switchControlProtocol();
            break;
        case STATUS_PROTOCOL:
            std::cout << "\nSTATUSPROTOCOL\n" << std::endl;
            switchStatusProtocol();
            break;
        case TERM_PROTOCOL:
            std::cout << "\nTERMPROTOCOL\n" << std::endl;
            switchThermalProtocol();
            break;
        default:
        // ignora
        break;
    }

}

void switchHealthProtocol() {
    switch (gsPacket.operation) {
        case GENERATE_HEALTH_DATA:
            std::cout << "GENERATE_HEALTH_DATA" << std::endl;
            generateHealthData();
            hStatus.length = sizeof(HealthStatus);
            hStatus.protocol = HEALTH_PROTOCOL;
            hStatus.operation = GENERATE_HEALTH_DATA;
            hStatus.numberOfPackages = HealthDataCounter;
            sendHealthStatus();
            sleep(5);
            break;

        case RESEND_HEALTH_DATA:
            std::cout << "RESEND HDATA" << std::endl;
            break;
        default:
        // ignora
        break;
    }
}

void switchAISProtocol(){
    
}

void switchMECProtocol(){
    switch(gsPacket.operation){
        case OPEN_ANTENNAS:
            break;
        default:
        // ignora
            break;
    }

}

void switchImagingProtocol(){

}

void switchControlProtocol(){
    switch(gsPacket.operation){
        case SOLAR_VECTOR:
            std::cout << "\nSOLAR_VECTOR\n" << std::endl;
            sleep(5);
            break;
        case TWO_VECTORS:
            std::cout << "\nTWO_VECTORS\n" << std::endl;
            std::cout << "Vetor 1: " << (int)gsPacket.vector1 << "\n";
            std::cout << "Vetor 2: " << (int)gsPacket.vector2 << "\n";
            sleep(5);
            break;
        case SUN_POINTING:
            std::cout << "\nSUN_POINTING\n" << std::endl;
            sleep(5);
            break;
        case STABILIZATION:
            std::cout << "\nSTABILIZATION\n" << std::endl;
            sleep(5);
            break;
        default:
        // ignora
        break;
    }
}

void switchStatusProtocol(){
    switch(gsPacket.operation){
        case INITCOMM:
            std::cout << "\nINITCOMM\n" << std::endl;
            satPacket.length = sizeof(SatPacket);
            satPacket.protocol = STATUS_PROTOCOL;
            satPacket.operation = INITCOMM;
            sendSatPacket();
            sleep(5);
            break;
        case VERIFY_FILE:
            std::cout << "\nVERIFY_FILE\n" << std::endl;
            verifyFile();
            break;
        case SHUT_DOWN_SYSTEM:
            std::cout << "\nSHUT DOWN SYSTEM\n" << std::endl;
            ShutDownSystem();
            break;
        default:
        // ignora
        break;
    }

}

void switchThermalProtocol(){
    switch(gsPacket.operation){
        case ACTIVATE_THERMAL_CONTROL:
            std::cout << "\nACTIVATE THERMAL CONTROL\n" << std::endl;
            activateThermalControl();
            break;
        case DEACTIVATE_THERMAL_CONTROL:
            std::cout << "\nDEACTIVATE THERMAL CONTROL\n" << std::endl;
            deactivateThermalControl();
            break;
        default:
        // ignora
        break;
    }
}