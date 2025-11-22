#include "Decodificação.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

NMEAData AISParser::parseNMEA(const std::string& nmeaSentence) {
    NMEAData result;
    result.rawSentence = nmeaSentence;
    
    if (nmeaSentence.empty() || (nmeaSentence[0] != '!' && nmeaSentence[0] != '$')) {
        return result;
    }
    
    std::vector<std::string> fields;
    std::stringstream ss(nmeaSentence);
    std::string field;
    
    // Dividir por vírgulas
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    
    if (fields.size() < 6) {
        return result;
    }
    
    // Tipo de sentença
    result.sentenceType = fields[0];
    
    // Verificar se é AIVDM ou AIVDO
    if (result.sentenceType != "!AIVDM" && result.sentenceType != "!AIVDO") {
        return result;
    }
    
    try {
        // Número do fragmento
        if (!fields[1].empty()) {
            result.fragmentNumber = std::stoi(fields[1]);
        }
        
        // Total de fragmentos
        if (!fields[2].empty()) {
            result.totalFragments = std::stoi(fields[2]);
        }
        
        // Canal
        if (!fields[3].empty()) {
            result.channel = fields[3][0];
        }
        
        // Dados codificados
        if (fields.size() > 5 && !fields[5].empty()) {
            result.encodedData = fields[5];
        }
        
        result.isValid = true;
        
    } catch (const std::exception& e) {
        std::cerr << "Erro no parsing NMEA: " << e.what() << std::endl;
    }
    
    return result;
}

AISData AISParser::decodeAIS(const NMEAData& nmeaData) {
    AISData result;
    
    if (!nmeaData.isValid || nmeaData.encodedData.empty()) {
        return result;
    }
    
    try {
        // Decodificar dados AIS de 6-bit para binário
        std::string binaryData;
        for (char c : nmeaData.encodedData) {
            if (c < 48 || c > 119 || (c > 87 && c < 96)) {
                continue; // Caractere inválido
            }
            
            uint8_t value = decodeAISChar(c);
            for (int i = 5; i >= 0; i--) {
                binaryData += (value & (1 << i)) ? '1' : '0';
            }
        }
        
        if (binaryData.length() < 6) {
            return result;
        }
        
        // Decodificar tipo de mensagem (bits 0-5)
        int messageType = decodeAISInteger(binaryData, 0, 6);
        result.messageType = static_cast<AISData::MessageType>(messageType);
        
        // Decodificar MMSI (bits 8-37)
        result.mmsi = decodeAISInteger(binaryData, 8, 30);
        
        // Decodificar dados baseado no tipo de mensagem
        switch (messageType) {
            case 1:
            case 2:
            case 3:
                // Mensagens de posição Classe A
                result.latitude = decodeAISLatLong(binaryData, 89, 27) / 600000.0;
                result.longitude = decodeAISLatLong(binaryData, 61, 28) / 600000.0;
                result.speedOverGround = decodeAISInteger(binaryData, 50, 10) / 10.0;
                result.courseOverGround = decodeAISInteger(binaryData, 116, 12) / 10.0;
                result.trueHeading = decodeAISInteger(binaryData, 128, 9);
                result.isValid = true;
                break;
                
            case 5:
                // Dados estáticos
                result.vesselName = decodeAISString(binaryData, 143, 120);
                result.callSign = decodeAISString(binaryData, 263, 42);
                result.isValid = true;
                break;
                
            case 18:
                // Mensagens de posição Classe B
                result.latitude = decodeAISLatLong(binaryData, 143, 27) / 600000.0;
                result.longitude = decodeAISLatLong(binaryData, 115, 28) / 600000.0;
                result.speedOverGround = decodeAISInteger(binaryData, 50, 10) / 10.0;
                result.courseOverGround = decodeAISInteger(binaryData, 116, 12) / 10.0;
                result.isValid = true;
                break;
                
            default:
                // Outros tipos de mensagem
                result.isValid = true;
                break;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Erro na decodificação AIS: " << e.what() << std::endl;
    }
    
    return result;
}

uint8_t AISParser::decodeAISChar(char c) {
    if (c < 48) return 0;
    if (c < 56) return static_cast<uint8_t>(c - 48);
    if (c < 96) return static_cast<uint8_t>(c - 56);
    return static_cast<uint8_t>(c - 64);
}

int AISParser::decodeAISInteger(const std::string& data, int startBit, int length) {
    if (startBit + length > data.length()) {
        return 0;
    }
    
    int result = 0;
    for (int i = 0; i < length; i++) {
        if (data[startBit + i] == '1') {
            result |= (1 << (length - 1 - i));
        }
    }
    
    return result;
}

std::string AISParser::decodeAISString(const std::string& data, int startBit, int length) {
    std::string result;
    int charCount = length / 6;
    
    for (int i = 0; i < charCount; i++) {
        int charValue = decodeAISInteger(data, startBit + (i * 6), 6);
        if (charValue == 0) {
            break; // Fim da string
        }
        
        // Converter para caractere ASCII
        if (charValue < 32) {
            charValue += 64;
        }
        
        if (charValue >= 32 && charValue <= 95) {
            result += static_cast<char>(charValue);
        }
    }
    
    // Remover espaços em branco no final
    while (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    
    return result;
}

double AISParser::decodeAISLatLong(const std::string& data, int startBit, int length) {
    int value = decodeAISInteger(data, startBit, length);
    
    // Verificar se é valor válido
    if (value == 0 || value == (1 << length) - 1) {
        return 0.0;
    }
    
    // Ajustar para coordenadas negativas (longitude oeste/latitude sul)
    int maxValue = (1 << (length - 1)) - 1;
    if (value > maxValue) {
        value = value - (1 << length);
    }
    
    return static_cast<double>(value);
}