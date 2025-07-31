#include "Arduino.h"
#include "GamaSatComm.h"


uint8_t rxAddH = 0x01;
uint8_t rxAddL = 0x10;
uint8_t txAddH = 0x02;
uint8_t txAddL = 0x20;

uint8_t calculaCheck(const uint8_t* dados, uint8_t tamanho) {
  uint8_t soma = 0;
  for (uint8_t i = 0; i < tamanho; i++) {
    soma += dados[i];
  }
  return soma;
}


void iniciarComunicacaoComGroundStation() {
  String remetente = "GAMASAT";
  String mensagem = "hi, groundStation";
  String payload = remetente + ":" + mensagem;

  uint8_t length  = payload.length();
  const char* buffer = payload.c_str(); // ponteiro para os bytes da String

  uint8_t check = calculaCheck((const uint8_t*)buffer, length);
  
  LoRa.beginPacket();
  LoRa.write(txAddH);
  LoRa.write(txAddL);
  LoRa.write(rxAddH);
  LoRa.write(rxAddL);
  LoRa.write(length);
  LoRa.write((const uint8_t*)buffer, length);
  LoRa.write(check);
  LoRa.endPacket();
  
  Serial.print("Pacote de resposta enviado:");
  Serial.print("Pacote completo: ");
  Serial.print(length + 6);
  Serial.print(" bytes\n");
  Serial.print("Payload: ");
  Serial.print(length);
  Serial.print(" bytes\n");
}