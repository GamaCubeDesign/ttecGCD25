#include <iostream>
#include <csignal>
#include <atomic>
#include <wiringSerial.h>
#include "Receiver.h"
#include "Decodificação.h"
#include "Dados.h"

std::atomic<bool> running{true};

void signalHandler(int signal) {
    std::cout << "\nRecebido sinal " << signal << ". Encerrando..." << std::endl;
    running = false;
}

void printBanner() {
    std::cout << "==========================================" << std::endl;
    std::cout << "    RECEPTOR AIS - Raspberry Pi Zero 2W" << std::endl;
    std::cout << "      Módulo GNS 5851 - 162 MHz" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Configuração:" << std::endl;
    std::cout << "  Porta Serial: /dev/ttyS0" << std::endl;
    std::cout << "  Baud Rate: 38400" << std::endl;
    std::cout << "  GPIO RX: 15 (wiringPi)" << std::endl;
    std::cout << "  Data Directory: /home/pi/ais_data" << std::endl;
    std::cout << "==========================================" << std::endl;
}

int main() {
    // Configurar handlers de sinal
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    printBanner();
    
    // Inicializar wiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "ERRO: Falha ao inicializar wiringPi!" << std::endl;
        return -1;
    }
    
    try {
        // Inicializar receptor AIS
        AISReceiver receiver("/dev/ttyS0", 38400);
        
        if (!receiver.initialize()) {
            std::cerr << "ERRO: Falha ao inicializar receptor AIS!" << std::endl;
            return -1;
        }
        
        // Inicializar data logger
        DataLogger dataLogger;
        if (!dataLogger.initialize()) {
            std::cerr << "ERRO: Falha ao inicializar DataLogger!" << std::endl;
            return -1;
        }
        
        std::cout << "Sistema AIS inicializado com sucesso!" << std::endl;
        std::cout << "Aguardando dados AIS... (Pressione Ctrl+C para parar)" << std::endl;
        
        AISParser parser;
        int messageCount = 0;
        int savedMessages = 0;
        
        // Loop principal
        while (running) {
            std::string message = receiver.readMessage();
            
            if (!message.empty()) {
                messageCount++;
                
                // Salvar mensagem NMEA crua
                dataLogger.logRawNMEA(message);
                
                // Processar mensagem NMEA
                NMEAData nmeaData = parser.parseNMEA(message);
                
                if (nmeaData.isValid) {
                    std::cout << "\n--- Mensagem AIS #" << messageCount << " ---" << std::endl;
                    std::cout << "Tipo: " << nmeaData.sentenceType << std::endl;
                    std::cout << "Raw: " << message << std::endl;
                    
                    // Tentar decodificar dados AIS
                    AISData aisData = parser.decodeAIS(nmeaData);
                    if (aisData.isValid) {
                        std::cout << "MMSI: " << aisData.mmsi << std::endl;
                        std::cout << "Tipo Mensagem: " << static_cast<int>(aisData.messageType) << std::endl;
                        
                        if (aisData.latitude != 0.0 && aisData.longitude != 0.0) {
                            std::cout << "Posição: " << aisData.latitude << ", " << aisData.longitude << std::endl;
                        }
                        
                        if (aisData.speedOverGround != 0.0) {
                            std::cout << "Velocidade: " << aisData.speedOverGround << " nós" << std::endl;
                        }
                        
                        if (aisData.trueHeading != 0) {
                            std::cout << "Curso: " << aisData.trueHeading << "°" << std::endl;
                        }
                        
                        if (!aisData.vesselName.empty()) {
                            std::cout << "Nome: " << aisData.vesselName << std::endl;
                        }
                        
                        // Salvar dados no JSON
                        dataLogger.logAISData(aisData, message);
                        savedMessages++;
                        
                        std::cout << "Dados salvos no JSON (" << savedMessages << " mensagens salvas)" << std::endl;
                    }
                }
                
                // Pequeno delay para não sobrecarregar a CPU
                delay(10);
            } else {
                // Timeout - verificar se ainda estamos recebendo dados
                static auto lastMessageTime = std::chrono::steady_clock::now();
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastMessageTime);
                
                if (elapsed.count() > 30) {
                    std::cout << "Aguardando sinais AIS... [" << messageCount << " mensagens recebidas, " 
                              << savedMessages << " salvas]" << std::endl;
                    lastMessageTime = now;
                }
                
                delay(100);
            }
        }
        
        std::cout << "Encerrando receptor AIS..." << std::endl;
        std::cout << "Total: " << messageCount << " mensagens recebidas, " 
                  << savedMessages << " salvas em JSON" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Erro fatal: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "Receptor AIS encerrado." << std::endl;
    return 0;
}