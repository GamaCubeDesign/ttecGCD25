#include "CommunicationProtocol.h"
#include <LoRa.h>

GSPacket packet;
SatPacket resposta;

void sendPacket(Protocol protocol, Operation operation) {

    packet.length = sizeof(GSPacket);
    packet.protocol = protocol;
    packet.operation = operation;
    
    Serial.print("}-->\n\n");

    Serial.print("PACKAGE GENERATED: \n\n");

    Serial.print("SIZE: ");
    Serial.println(packet.length);

    Serial.print("PROTOCOL: ");
    Serial.println(packet.protocol);

    Serial.print("OPERATION: ");
    Serial.println(packet.operation);

    uint8_t *data = (uint8_t*)&packet;

    Serial.print("Sending Packet: ");
    Serial.print("{");
    for (int i = 0; i < packet.length; i++) {
        Serial.print((int)data[i]);
        Serial.print(" ");
    }
    Serial.println("}");
    

    LoRa.beginPacket();
    LoRa.write(data, packet.length);
    LoRa.endPacket();

    Serial.println("Package sent!\n");
    Serial.print("}-->\n\n");

    return;
}

bool receivePacket(SatPacket *packet, unsigned long timeout_ms) {
    unsigned long start = millis();

    while (millis() - start < timeout_ms) {
        int packetSize = LoRa.parsePacket();
        if (packetSize == 0) continue;  // Nenhum pacote ainda

        Serial.println("\n<--{ PACKET RECEIVED }");

        if (packetSize != sizeof(SatPacket)) {
            Serial.println("Packet size mismatch!");
            Serial.print("Expected: "); Serial.println(sizeof(SatPacket));
            Serial.print("Received: "); Serial.println(packetSize);
            while (LoRa.available()) LoRa.read(); // limpa buffer
            return false;
        }

        uint8_t buffer[sizeof(SatPacket)];
        int index = 0;
        while (LoRa.available() && index < sizeof(SatPacket)) {
            buffer[index++] = (uint8_t)LoRa.read();
        }

        memcpy(packet, buffer, sizeof(SatPacket));

        Serial.println("Decoded Packet Content:");
        Serial.print("SIZE: "); Serial.println(packet->length);
        Serial.print("PROTOCOL: "); Serial.println(packet->protocol);
        Serial.print("OPERATION: "); Serial.println(packet->operation);

        Serial.print("Packet Bytes: { ");
        for (int i = 0; i < sizeof(SatPacket); i++) {
            Serial.print((int)buffer[i]);
            Serial.print(" ");
        }
        Serial.println("}");
        Serial.println("<--}\n");

        return true;
    }

    Serial.println("Timeout: no response received.\n");
    return false;
}

void onReceive() {
    Serial.println("\nProcessing received packet...");

    switch (resposta.protocol) {
        case HEALTH_PROTOCOL:
            Serial.println("Protocol: HEALTH_PROTOCOL");
            switch (resposta.operation) {
                case GENERATE_HEALTH_DATA:
                
                    break;

                default:
                    Serial.println("Operação desconhecida no protocolo HEALTH_PROTOCOL");
                    break;
            }
            break;
        
            case STATUS_PROTOCOL:
            Serial.println("Protocol: STATUS_PROTOCOL");
            switch (resposta.operation) {
                case INITCOMM:
                    Serial.println("GAMASAT -> HI GROUND!!");
                    break;

                default:
                    Serial.println("Operação desconhecida no protocolo STATUS_PROTOCOL");
                    break;
            }
            break;

        default:
            Serial.println("Protocolo desconhecido!");
            break;
    }

    Serial.println("Packet processed.\n");
}
