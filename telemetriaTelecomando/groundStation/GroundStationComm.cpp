#include "Arduino.h"
#include "GroundStationComm.h"

uint8_t txAddH = 0x01;
uint8_t txAddL = 0x10;
uint8_t rxAddH = 0x02;
uint8_t rxAddL = 0x20;

#define rxHSAT 0x01
#define rxLSAT 0x10

uint8_t calculaCheck(const uint8_t* dados, uint8_t tamanho) {
  uint8_t soma = 0;
  for (uint8_t i = 0; i < tamanho; i++) {
    soma += dados[i];
  }
  return soma;
}

bool receberPacote(char *mensagem){
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return false;
    
    Serial.print("Recebendo e lendo pacote loRa");
    
    uint8_t txH = LoRa.read();
    uint8_t txL = LoRa.read();
    uint8_t rxH = LoRa.read();
    uint8_t rxL = LoRa.read();
    uint8_t length = LoRa.read();

  if (rxH != rxHSAT || rxL != rxLSAT) {
      Serial.println("Pacote não foi destinado para o Gama Sat");
      while (LoRa.available()) LoRa.read();
      return false;
    }

    
  if (length != packetSize - 6) {
    Serial.println("Tamanho do pacote não confere");
    while (LoRa.available()) LoRa.read();
    return false;
  }
  
  uint8_t checksum = 0;

  for (int i = 0; i < length; i++) {
    mensagem[i] = (char)LoRa.read();     
    checksum += mensagem[i];         
  }
  mensagem[length] = '\0'; 

  uint8_t checkRecebido = LoRa.read();
  if (checksum != checkRecebido) {
    Serial.println("Check inválido");
    return false;
  }

    Serial.println("Pacote recebido com sucesso:");
    Serial.print("packetsize: ");
    Serial.print(packetSize);
    Serial.print(" bytes");
    Serial.print("   Payload: ");
    Serial.print(length);
    Serial.print(" bytes\n");
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi()); 
    Serial.print("Mensagem recebida: ");
    Serial.println(mensagem);
    return true;
}

void iniciarComunicacaoComSatelite() {
  char mensagemRecebida[256];
  String remetente = "GSGAMA";
  String mensagem = "Olá, GamaSat";
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

  Serial.print("Pacote enviado de ");
  Serial.print(length + 6);
  Serial.print(" bytes\n");
  Serial.print("Payload enviado de ");
  Serial.print(length);
  Serial.print(" bytes\n");


  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 5000) {
    if (receberPacote(mensagemRecebida)) {
      if (strcmp(mensagemRecebida, "GAMASAT:Olá, groundStation") == 0) {
        Serial.println("GamaSat respondeu");
        return; 
      }
    }
  }

  Serial.println("\nERRO: GamaSat não respondeu dentro do tempo de 5 segundos.");
}