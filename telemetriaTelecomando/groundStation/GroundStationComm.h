#ifndef GROUNDSTATIONCOMM_H
#define GROUNDSTATIONCOMM_H

#include <SPI.h>              
#include <LoRa.h>

uint8_t calculaCheck(const uint8_t* dados, uint8_t tamanho);
bool receberPacote(char *mensagem);
void iniciarComunicacaoComSatelite();


#endif