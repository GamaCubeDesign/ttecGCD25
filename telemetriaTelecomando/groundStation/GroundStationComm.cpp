#include "Arduino.h"
#include "GroundStationComm.h"

uint8_t txAddH = 0x01;
uint8_t txAddL = 0x10;
uint8_t rxAddH = 0x02;
uint8_t rxAddL = 0x20;

void iniciarComunicacaoComSatelite() {
  String remetente = "GSGAMA";
  String mensagem = "Olá, GamaSat";
  String payload = remetente + ":" + mensagem;

  uint8_t length  = payload.length();
  const char* buffer = payload.c_str(); // ponteiro para os bytes da String


  
  LoRa.beginPacket();
  LoRa.write(txAddH);
  LoRa.write(txAddL);
  LoRa.write(rxAddH);
  LoRa.write(rxAddL);
  LoRa.write(length);
  LoRa.write((const uint8_t*)buffer, length);
  LoRa.endPacket();

  Serial.print("Pacote enviado de ");
  Serial.print(length + 5);
  Serial.print(" bytes\n");
  Serial.print("Payload enviado de ");
  Serial.print(length);
  Serial.print(" bytes\n");


  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 5000) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String resposta = "";
      while (LoRa.available()) {
        resposta += (char)LoRa.read();
      }

      if (resposta.equals("GAMASAT:Olá, groundStation")) {
        Serial.println("GAMASAT: Olá, groundStation!");
      } else {
        Serial.print("Resposta inesperada: ");
        Serial.println(resposta);
      }
      return;
    }
  }

  Serial.println("\nERRO: GamaSat não respondeu dentro do tempo de 5 segundos.");
}