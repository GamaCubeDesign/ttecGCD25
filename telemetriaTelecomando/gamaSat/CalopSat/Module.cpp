#include "Module.h"
#include "Moden.h"
#include "Integration.h"
#include <iostream>


bool talking = false;


unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;

SatPacket satPacket;
GSPacket gsPacket;
uint8_t rx_pointer = 0;

void sendSatPacket() {
    //tx_send((uint8_t*)&satPacket, satPacket.length);
    std::cout << "Protocol: " << satPacket.protocol << "\n";
    std::cout << "Operation: " << satPacket.operation << "\n";
}

/*void initcomm(){
    memset(&satPacket, 0, sizeof(SatPacket)); // zera memória da struct
    const char *mensagem = "Olá, groundstation!";
    // copia texto e define tamanho
    strncpy(satPacket.txt, mensagem, sizeof(satPacket.txt) - 1);
    satPacket.length = sizeof(SatPacket);
    sendSatPacket();
}*/

/*
void updateRFComm() {
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
}*/


//Essa função decide o que fazer de acordo com o comando recebido
void onReceive() {
    std::cout << "Protocol:" << (int)gsPacket.protocol << std::endl;
    std::cout << "Operation:" << (int)gsPacket.operation << std::endl;
    
    switch (gsPacket.protocol) {
        case HEALTH_PROTOCOL:
            std::cout << "\nHEALTHPROTOCOL\n" << std::endl;
            switchHealthProtocol();
            break;
        case AIS_PROTOCOL:
            std::cout << "\nAISPROTOCOL\n" << std::endl;
            
            break;
        case MEC_PROTOCOL:
            std::cout << "\nMECPROTOCOL\n" << std::endl;
            
            break;
        case STATUS_PROTOCOL:
            std::cout << "\nSTATUSPROTOCOL\n" << std::endl;
            switchStatusProtocol();
            break;
        default:
        // ignorar ou logar erro
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
            break;
        case VERIFY_FILE:
            std::cout << "\nVERIFY_FILE\n" << std::endl;
            //int verifyFile();
        default:
        // ignorar ou logar erro
        break;
    }

}



void switchHealthProtocol() {
    switch (gsPacket.operation) {
        case GENERATE_HEALTH_DATA:
            std::cout << "GENERATE_HEALTH_DATA" << std::endl;
            generateHealthData();
            break;

        case RESEND_HEALTH_DATA:
            
            break;
        default:
        // ignorar ou logar erro
        break;
    }
}

/*
int main(){
    satPacket.protocol = STATUS_PROTOCOL;
    satPacket.operation = INITCOMM;
    onReceive();
    return 0;
}*/
