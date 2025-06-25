#include "Arduino.h"
#include "GamaSatComm.h"

//Falta inverter esses endereços.
uint8_t txAddH = 0x01;
uint8_t txAddL = 0x10;
uint8_t rxAddH = 0x02;
uint8_t rxAddL = 0x20;


void iniciarComunicacaoComGroundStation() {
  String remetente = "GAMASAT";
  String mensagem = "Olá, groundStation";
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

  display.clearDisplay();
  display.setCursor(0, 0); 
  display.println("Pacote de resposta enviado:")
  display.print("Pacote completo: ");
  display.print(length + 6);
  display.println(" bytes");
  display.print("Payload: ");
  display.print(length);
  display.println(" bytes");
  display.display();
}