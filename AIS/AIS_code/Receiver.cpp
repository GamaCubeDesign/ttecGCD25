#include "Receiver.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdint>  // Adicionado para uint8_t

AISReceiver::AISReceiver(const std::string& port, int baud)
    : serialPort(port), baudRate(baud), serialHandle(-1) {}

AISReceiver::~AISReceiver() {
    if (serialHandle != -1) {
        serialClose(serialHandle);
    }
}

bool AISReceiver::initialize() {
    serialHandle = serialOpen(serialPort.c_str(), baudRate);

    if (serialHandle == -1) {
        std::cerr << "ERRO: Não foi possível abrir a porta serial " 
                  << serialPort << std::endl;
        return false;
    }

    std::cout << "Porta serial " << serialPort 
              << " aberta com sucesso (baud: " << baudRate << ")" << std::endl;

    serialFlush(serialHandle);
    return true;
}

std::string AISReceiver::readMessage() {
    if (serialHandle == -1) {
        return "";
    }

    static std::string buffer;  // buffer 
    char c;

    while (serialDataAvail(serialHandle) > 0) {
        c = serialGetchar(serialHandle);

        // Armazena caractere
        buffer += c;

        // Detectar fim da sentença
        if (c == '\n') {
            std::string message = buffer;
            buffer.clear();

            // Remover espaços e CR
            message.erase(
                std::remove(message.begin(), message.end(), '\r'),
                message.end()
            );
            message.erase(
                std::remove(message.begin(), message.end(), '\n'),
                message.end()
            );

            if (message.empty()) return "";

            // Validar se inicia com ! ou $
            if (message[0] != '!' && message[0] != '$') {
                std::cerr << "Mensagem inválida (sem prefixo NMEA): " 
                          << message << std::endl;
                return "";
            }

            // Validar checksum
            if (validateChecksum(message)) {
                return message;
            } else {
                std::cerr << "Checksum inválido: " << message << std::endl;
                return "";
            }
        }
    }

    return "";
}

void AISReceiver::sendCommand(const std::string& command) {
    if (serialHandle != -1) {
        serialPuts(serialHandle, command.c_str());
        serialPuts(serialHandle, "\r\n");
    }
}

bool AISReceiver::validateChecksum(const std::string& nmeaSentence) {
    size_t asteriskPos = nmeaSentence.find('*');  // Removido ; extra
    if (asteriskPos == std::string::npos || asteriskPos + 2 >= nmeaSentence.size()) {
        return false;
    }

    // Dados entre o prefixo e o *
    std::string data = nmeaSentence.substr(1, asteriskPos - 1);

    // Checksum recebido (2 hex dígitos)
    std::string receivedChecksum = nmeaSentence.substr(asteriskPos + 1, 2);

    // Calcular checksum XOR
    uint8_t calc = 0;
    for (char c : data) {
        calc ^= c;
    }

    // Converter para HEX
    char calcStr[3];
    snprintf(calcStr, sizeof(calcStr), "%02X", calc);

    return receivedChecksum == calcStr;
}