#include "Arduino.h"
#include "GamaSatComm.h"

void iniciarComunicacaoComGroundStation() {
  String remetente = "GAMASAT";
  String mensagem = "Olá, groundStation";
  String pacote = remetente + ":" + mensagem;
  
  LoRa.beginPacket();
  LoRa.print(pacote);
  LoRa.endPacket();
}