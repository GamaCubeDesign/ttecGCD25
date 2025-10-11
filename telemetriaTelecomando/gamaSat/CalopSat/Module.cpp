#include "Module.h"
#include <iostream>


bool talking = false;


unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;

SatPacket satPacket;
GSPacket gsPacket;
uint8_t rx_pointer = 0;

void sendSatPacket() {
    tx_send((uint8_t*)&satPacket, satPacket.length);
}

void initcomm(){
    memset(&satPacket, 0, sizeof(SatPacket)); // zera memória da struct
    const char *mensagem = "Olá, groundstation!";
    // copia texto e define tamanho
    strncpy(satPacket.txt, mensagem, sizeof(satPacket.txt) - 1);
    satPacket.length = sizeof(SatPacket);
    sendSatPacket();
}


void updateRFComm() {
    uint8_t b;
    if (modemAvailable()) {
        while (modemAvailable()) {
            b = modemRead();
            ((uint8_t*)(&gsPacket))[rx_pointer++] = b; //transforma a struct em array de bytes e vai colocando cada byte no seu lugar.

            if (rx_pointer > 0 && rx_pointer == gsPacket.length) {
                std::cout << "Received gs: ";
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

        communication_timeout = millis() + communication_timeout_limit;
    }
}


//Essa função decide o que fazer de acordo com o comando recebido
void onReceive() {
    std::cout << "lenght" << (int)gsPacket.length<< std::endl;
    std::cout << "txt:" << gsPacket.txt<< std::endl;

    if (strcmp(gsPacket.txt, "Olá, gamasat!") == 0) {
        std::cout << "Ground: Olá, gamasat!" << std::endl;
        void initcomm();
    }
    else if (strcmp(gsPacket.txt, "status?") == 0) {
        std::cout << "Comando de status recebido!" << std::endl;
    }
    else if (strcmp(gsPacket.txt, "ping") == 0) {
        std::cout << "Ping recebido, respondendo pong!" << std::endl;
    }
    else {
        std::cout << "Mensagem desconhecida." << std::endl;
    }
}



