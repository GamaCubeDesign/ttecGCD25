#include "Arduino.h"
#include "GroundStationComm.h"

void iniciarComunicacaoComSatelite() {
  String remetente = "GSGAMA";
  String mensagem = "Olá, GamaSat";
  String pacote = remetente + ":" + mensagem;
  
  LoRa.beginPacket();
  LoRa.print(pacote);
  LoRa.endPacket();

  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 5000) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String resposta = "";
      while (LoRa.available()) {
        resposta += (char)LoRa.read();
      }

      if (resposta.equals("GAMASAT:Olá, groundStation")) {
        Serial.println("GamaSat respondeu corretamente!");
      } else {
        Serial.print("Resposta inesperada: ");
        Serial.println(resposta);
      }
      return;
    }
  }

  Serial.println("Erro: GamaSat não respondeu dentro do tempo.");
}