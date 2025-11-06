#include "Integration.h"
#include "CommunicationProtocol.h"

#include "Moden.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


healthData health;
imagingData imaging;

fila* criaFila() {
    fila* f = (fila*) malloc(sizeof(fila));
    if (!f) return NULL;
    f->prox = f;
    return f;
}

fila* enfileira(fila* f, healthData x) {
    fila* novo = (fila*) malloc(sizeof(fila));
    if (!novo) return f;
    novo -> prox = f -> prox;
    f -> prox = novo;
    f -> dado = x;
    return novo;
}

// Desinfileira do início
int desinfileira(fila* f, healthData* y) {
    if (f->prox == f) return 1; // fila vazia
    fila* lixo = f->prox;
    *y = lixo->dado;
    f->prox = lixo->prox;
    free(lixo);
    return 0; // sucesso
}

void parseHealth(){
    std::ifstream file("sensor_data.json");
    json j;
    file >> j;

    health.voltage = j["sensors"]["INA219"].value("voltage_V", 0.0f);
    health.current = j["sensors"]["INA219"].value("current_mA", 0.0f);
    health.power = j["sensors"]["INA219"].value("power_mW", 0.0f);
    health.shunt = j["sensors"]["INA219"].value("shunt_voltage_mV", 0.0f);
    
    health.temperature = j["sensors"]["BMP180"].value("temperature_C", 0.0f);
    health.pressure = j["sensors"]["BMP180"].value("pressure_hPa", 0.0f);
    health.altitude = j["sensors"]["BMP180"].value("altitude_m", 0.0f);
    
    health.temperatureD = j["sensors"]["DS18B20"].value("temperature_C", 0.0f);

    health.sd_memory_usage = 100;

    health.length = sizeof(health);

    std::cout << "Voltage INA219: " << health.voltage << " V\n";
    std::cout << "Currente INA219: " << health.current << " mA\n";
    std::cout << "Power INA219: " << health.power << " mW\n";
    std::cout << "Shunt INA219: " << health.shunt << " mV\n";

    std::cout << "Temperature BMP180: " << health.temperature << " °C\n";
    std::cout << "Pressure BMP180: " << health.pressure << " hPa\n";
    std::cout << "Altitude BMP180: " << health.altitude << " m\n";
    
    std::cout << "Temperature DS18B20: " << health.temperatureD << " °C\n";

    std::cout << "Memory: " << health.sd_memory_usage << " mb\n";
}
//Função para simular dados
void parse2Health(){
    std::ifstream file("sensor_data2.json");
    json j;
    file >> j;

    health.voltage = j["sensors"]["INA219"].value("voltage_V", 0.0f);
    health.current = j["sensors"]["INA219"].value("current_mA", 0.0f);
    health.power = j["sensors"]["INA219"].value("power_mW", 0.0f);
    health.shunt = j["sensors"]["INA219"].value("shunt_voltage_mV", 0.0f);
    
    health.temperature = j["sensors"]["BMP180"].value("temperature_C", 0.0f);
    health.pressure = j["sensors"]["BMP180"].value("pressure_hPa", 0.0f);
    health.altitude = j["sensors"]["BMP180"].value("altitude_m", 0.0f);
    
    health.temperatureD = j["sensors"]["DS18B20"].value("temperature_C", 0.0f);

    health.sd_memory_usage = 352;

    health.length = sizeof(health);

    std::cout << "Voltage INA219: " << health.voltage << " V\n";
    std::cout << "Currente INA219: " << health.current << " mA\n";
    std::cout << "Power INA219: " << health.power << " mW\n";
    std::cout << "Shunt INA219: " << health.shunt << " mV\n";

    std::cout << "Temperature BMP180: " << health.temperature << " °C\n";
    std::cout << "Pressure BMP180: " << health.pressure << " hPa\n";
    std::cout << "Altitude BMP180: " << health.altitude << " m\n";
    
    std::cout << "Temperature DS18B20: " << health.temperatureD << " °C\n";

    std::cout << "Memory: " << health.sd_memory_usage << " mb\n";
}

void sendHealthData(){
    tx_send((uint8_t*)&health, health.length);
    std::cout << "\nHealth data sent.\n" << std::endl;

}


void parseImaging(){
    imaging.length = sizeof(imaging);
}


void SendImagingData(){
    tx_send((uint8_t*)&imaging, imaging.length);
}

int verifyFile(){
    std::string fileName = "sensor_data.json";

    FILE *file = fopen(fileName.c_str(), "r"); 

    if (file != NULL) {
        std::cout << "The file exists\n\n";
        std::fclose(file);
        return 1;
    }else {
        std::cout << "The file does not exist\n";
        return 0;
    }
}

void generateHealthData(){
    //Falta adicionar a execução do código de OBC para gerar o .json
    
    if(verifyFile()){
        parseHealth();
        sendHealthData();
    } else {
        std::cout << "Health data file not found." << std::endl;
    }
}

void mostraFIFO(fila* f) {
    if (!f || f->prox == f) {
        std::cout << "Fila vazia!" << std::endl;
        std::cout << "               _____________________ " << std::endl;
        std::cout << "              |                     |" << std::endl;
        std::cout << "              V                     |" << std::endl;
        std::cout << "                                    |" << std::endl;
        std::cout << "          Nó Cabeça                 |" << std::endl;
        std::cout << "                                    |" << std::endl;
        std::cout << "              |_____________________|" << std::endl;
        std::cout << "" << std::endl;

        return;
    }

    fila* atual = f->prox;
    int i = 0;

    
    std::cout << "               _____________________ " << std::endl;
    std::cout << "              |                     |" << std::endl;
    std::cout << "              V                     |" << std::endl;
    while (atual != f) { // percorre até voltar ao nó cabeça
        std::cout << "                                    |" << std::endl;
        std::cout << "  Pacote " << i << ":                         |" << std::endl;
        std::cout << "  Voltage: " << atual->dado.voltage << " V                  |" << std::endl;
        std::cout << "  Current: " << atual->dado.current << " mA                 |" << std::endl;
        std::cout << "  Power: " << atual->dado.power << " mW                  |" << std::endl;
        std::cout << "  Shunt: " << atual->dado.shunt << " mV                    |" << std::endl;
        std::cout << "  Temperature BMP180: " << atual->dado.temperature << " °C       |" << std::endl;
        std::cout << "  Pressure BMP180: " << atual->dado.pressure << " hPa       |" << std::endl;
        std::cout << "  Altitude BMP180: " << atual->dado.altitude << " m          |" << std::endl;
        std::cout << "  Temperature DS18B20: " << atual->dado.temperatureD << " °C      |" << std::endl;
        std::cout << "  SD Memory Usage: " << atual->dado.sd_memory_usage << " MB           |" << std::endl;
        std::cout << "                                    |" << std::endl;
        std::cout << "              |                     |" << std::endl;
        std::cout << "              V                     |" << std::endl;
        atual = atual->prox;
        i++;
    }
    std::cout << "                                    |" << std::endl;
    std::cout << "          Nó Cabeça                 |" << std::endl;
    std::cout << "                                    |" << std::endl;
    std::cout << "              |_____________________|" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;

}



/*
int main(){
    healthData removed;
    generateHealthData();
    fila* f = criaFila();

    mostraFIFO(f);                  
    f = enfileira(f, health);
    
    parse2Health();
    f = enfileira(f, health);

    mostraFIFO(f);                  

    desinfileira(f, &removed);

    mostraFIFO(f);                  
    free(f);
    return 0;
}*/
