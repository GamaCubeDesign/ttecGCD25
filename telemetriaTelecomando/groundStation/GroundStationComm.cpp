#include "Arduino.h"
#include "GroundStationComm.h"

uint8_t txAddH = 0x01;
uint8_t txAddL = 0x10;
uint8_t rxAddH = 0x02;
uint8_t rxAddL = 0x20;

uint8_t calculaCheck(const uint8_t* dados, uint8_t size) {
  uint8_t soma = 0;
  for (uint8_t i = 0; i < size; i++) {
    soma += dados[i];
  }
  return soma;
}


bool enviarPacote(String payload) {
  uint8_t length = payload.length();
  const char* buffer = payload.c_str();

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
  
  Serial.println(" ");
  Serial.println("#");
  Serial.println("Package sent:");
  Serial.print("txAddH: ");
  Serial.println(txAddH);
  Serial.print("txAddL: ");
  Serial.println(txAddL);
  Serial.print("Payload: ");
  Serial.println(payload);
  Serial.print("Payload size: ");
  Serial.print(length);
  Serial.println(" bytes");
  Serial.print("Checksum: ");
  Serial.println(check);
  Serial.print("total package size: ");
  Serial.print(length + 6); 
  Serial.println(" bytes");
  Serial.println("#");
  Serial.println(" ");

  return true;
}


bool receberPacote(char *mensagem){
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return false;
    
    Serial.print("Receiving and reading loRa packet");
    
    uint8_t txH = LoRa.read();
    uint8_t txL = LoRa.read();
    uint8_t rxH = LoRa.read();
    uint8_t rxL = LoRa.read();
    uint8_t length = LoRa.read();

  if (rxH != 0x01 || rxL != 0x10) {
      Serial.println("Package was not intended for Gama GroundStation");
      while (LoRa.available()) LoRa.read();
      return false;
    }

    
  if (length != packetSize - 6) {
    Serial.println("Incorrect package size");
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
    Serial.println("Invalid check");
    return false;
  }

    Serial.println("Package received successfully:");
    Serial.print("packetsize: ");
    Serial.print(packetSize);
    Serial.print(" bytes");
    Serial.print("   Payload: ");
    Serial.print(length);
    Serial.print(" bytes\n");
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi()); 
    Serial.print("Message received:");
    Serial.println(mensagem);
    return true;
}

void iniciarComunicacaoComSatelite() {
  char mensagemRecebida[256];
  String remetente = "GSGAMA";
  String mensagem = "Olá, GamaSat";
  String payload = remetente + ":" + mensagem;
  
  enviarPacote(payload);

  Serial.println("waiting");

  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 5000) {
    if (receberPacote(mensagemRecebida)) {
      if (strcmp(mensagemRecebida, "GAMASAT:Olá, groundStation") == 0) {
        Serial.println("GamaSat #> hi, groundStation");
        return; 
      }
    }
  }

  Serial.println("\nERROR 505");
}