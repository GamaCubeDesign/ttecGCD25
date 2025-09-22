#ifndef GAMASATCOMM_H
#define GAMASATCOMM_H

#include <SPI.h>              
#include <LoRa.h>

uint8_t calculaCheck(const uint8_t* dados, uint8_t tamanho);
void iniciarComunicacaoComGroundStation();
bool enviarPacote(String payload);
void hdata();



#endif