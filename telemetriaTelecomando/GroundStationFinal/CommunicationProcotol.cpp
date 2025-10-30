#include "CommunicationProtocol.h"
#include <LoRa.h>

// Função para enviar um pacote via LoRa
void sendPacket(Protocol protocol, Operation operation) {
    GSPacket packet;

    packet.length = sizeof(GSPacket);
    packet.protocol = protocol;
    packet.operation = operation;

    uint8_t *data = (uint8_t*)&packet;

    Serial.print("Enviando pacote via LoRa: ");
    for (int i = 0; i < packet.length; i++) {
        Serial.print((int)data[i]);
        Serial.print(" ");
    }
    Serial.println("");

    LoRa.beginPacket();
    LoRa.write(data, packet.length);
    LoRa.endPacket();

    Serial.println("Pacote enviado!\n");
}