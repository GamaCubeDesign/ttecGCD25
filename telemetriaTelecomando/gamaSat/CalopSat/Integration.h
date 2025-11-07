#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <stdint.h>
#include <stdlib.h>

struct healthData {
    float voltage;
    float current;
    float power;
    float shunt;
    float temperature;
    float pressure;
    float altitude;
    float temperatureD;
    unsigned long int sd_memory_usage;
    uint8_t length;
};

extern healthData health;


struct imagingData {
    uint8_t length;
};

typedef struct celula {
    healthData dado;
    struct celula* prox;
} fila;

void parseHealth();
void parse2Health();
void parseImaging();
void sendHealthData();
void SendImagingData();
int verifyFile();
void generateHealthData();

extern fila* HealthFIFO;
extern fila* ImagingFIFO;
extern fila* ThermalControlFIFO;

extern int HealthDataCounter;
extern int ImagingDataCounter;
extern int ThermalControlDataCounter;

fila* CreateFIFO();
fila* Enqueue(fila* f, healthData x);
int Dequeue(fila* f, healthData* y);
void ShowFIFO(fila* f);

void initSubsystems();

#endif
