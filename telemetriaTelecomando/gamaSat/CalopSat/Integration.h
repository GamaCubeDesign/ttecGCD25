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

struct thermalControlData {
    float batteryTemperature1;
    float batteryTemperature2;
    float temperatureOut;
    uint8_t length;
};

extern thermalControlData thermalControlD;

struct imagingData {
    uint8_t length;
};

typedef struct celula {
    healthData dado;
    struct celula* prox;
} fila;

void parseHealth();
void parseImaging();
void parseControleTermico();
void parseControle();

void sendHealthData();
void SendImagingData();
void sendThermalControlData();
void sendControlData();

int verifyFile();
void RemoveFile();
void generateHealthData();

extern fila* HealthFIFO;
extern fila* ImagingFIFO;
extern fila* ThermalControlFIFO;

extern int HealthDataCounter;
extern int ImagingDataCounter;
extern int ThermalControlDataCounter;

extern const char *HealthFil;
extern const char *ImagingFile;
extern const char *ThermalControlFile;
extern const char *ControlFile;
extern const char *AISFile;

extern bool run;

fila* CreateFIFO();
fila* Enqueue(fila* f, healthData x);
int Dequeue(fila* f, healthData* y);
void DestroyFIFO(fila *f);
void ShowFIFO(fila* f);

void initSubsystems();
void ShutDownSystem();

void activateThermalControl();
void deactivateThermalControl();

#endif
