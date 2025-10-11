#include "Arduino.h"
#include "GroundStationComm.h"

struct SatPacket {
  uint8_t length;
  char txt[32];
};

SatPacket packet;

bool enviarPacote(const char* mensagem) {
  memset(&packet, 0, sizeof(SatPacket)); // zera memória da struct

  // copia texto e define tamanho
  strncpy(packet.txt, mensagem, sizeof(packet.txt) - 1);
  packet.length = strlen(packet.txt);


  // envia a struct inteira via LoRa
  LoRa.beginPacket();
  LoRa.write((uint8_t*)&packet, sizeof(SatPacket));
  LoRa.endPacket();

  Serial.println("Enviando pacote struct:");
  Serial.print("Tamanho total: ");
  Serial.println(sizeof(SatPacket));
  Serial.print("Length field: ");
  Serial.println(packet.length);
  Serial.print("Mensagem: ");
  Serial.println(packet.txt);
  Serial.println("###############################");
  Serial.println(" ");

  return true;
}


bool receberPacote(){
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return false;

  Serial.println("Pacote recebido!");

  // lê exatamente o tamanho da struct
  int bytesLidos = LoRa.readBytes((uint8_t*)&packet, sizeof(SatPacket));

  if (bytesLidos != sizeof(SatPacket)) {
    Serial.print("Tamanho incorreto! Esperado: ");
    Serial.print(sizeof(SatPacket));
    Serial.print(" bytes, recebido: ");
    Serial.println(bytesLidos);
    return false;
  }

  // imprime o conteúdo da struct
  Serial.print("Length: ");
  Serial.println(packet.length);
  Serial.print("Mensagem: ");
  Serial.println(packet.txt);


  memset(&packet, 0, sizeof(SatPacket)); // zera memória da struct


  return true;
}

void initcomm() {
  String payload = "Olá, gamasat!";
  
  enviarPacote(payload);

  Serial.println("waiting...");

  receberPacote();

}



void hdata(){
  char mensagemHD[256];
  memset(mensagemHD, 0, sizeof(mensagemHD));
  String payload = "hd";
  enviarPacote(payload);
  
  Serial.println("waiting...");

  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 20000) {
    if (receberPacote(mensagemHD)) {
      if (strcmp(mensagemHD, "Health data not found") == 0) {
        Serial.println("GamaSat has no health data to send");
        return; 
      }
    }
  }
  Serial.println("\nGamaSat did not respond correctly in the expected time (20s)");
}

void sms(String message){
  enviarPacote(message);
}

void camon(String payload){
  char mensagemCO[256];
  memset(mensagemCO, 0, sizeof(mensagemCO));
  enviarPacote(payload);


  Serial.println("The camera connection was requested.");
  Serial.println("Waiting for confirmation ...");

  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 20000) {
    if (receberPacote(mensagemCO)) {
      if (strcmp(mensagemCO, "Camera connected successfully.") == 0) {
        Serial.println("Camera connected successfully.");
        return;
        if (strcmp(mensagemCO, "Shutting down the camera.") == 0) {
        Serial.println("Camera shut down successfully.");
        return;  
      }
    }
  }
}
  Serial.println("\nGamaSat did not respond correctly in the expected time (20s)");
}

void antennaopen(String payload){
  char messageAT[256];
  memset(messageAT, 0, sizeof(messageAT));
  enviarPacote(payload);


  Serial.println("Antenna deployment was requested.");
  Serial.println("Waiting for confirmation ...");

  unsigned long starttime = millis();
  while (millis() - starttime < 20000) {
    if (receberPacote(messageAT)) {
      if (strcmp(messageAT, "The antennas were deployed successfully.") == 0) {
        Serial.println("The antennas were deployed successfully.");
        return;  
      }
    }
  }
  Serial.println("\nGamaSat did not respond correctly in the expected time (20s)");
}


