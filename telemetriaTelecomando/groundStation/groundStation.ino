#include <SPI.h>
#include <LoRa.h>
#include "GroundStationComm.h"

#define LORA_PIN_SCK      18     // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     23    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      5    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes)

bool inicializaLoRa() {
  SPI.begin(LORA_PIN_SCK, LORA_PIN_MISO, LORA_PIN_MOSI, LORA_PIN_NSS);
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);

  if (!LoRa.begin(433E6)) {
    Serial.println("Falha na inicialização do LoRa. Verifique conexões.");
    return false;
  }

  Serial.println("LoRa inicializado com sucesso.");
  LoRa.begin(433E6);  
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  
  delay(1000);


  Serial.println("Testing the Gama Ground Station communication system with the LoRa Ra-02 RF module");

  if (!inicializaLoRa()) {
    Serial.println("Tentando reinicializar em 5 segundos...");
    delay(5000);
    inicializaLoRa(); 
  }
}


void loop() {
  if (Serial.available()) {
    String comand = Serial.readStringUntil('\n');
    comand.trim();

    if (comand == "initcomm") {
      Serial.println("starting communication");
      initcomm();
    }
    else if (comand == "hdata") {
      Serial.println("requested health data");
      hdata();
    }
    else if (comand.startsWith("sms ")) {
      String message = comand.substring(4);
      message.trim();
      sms(message);
    }
    else {
      Serial.println("Error: nonexistent command.");
    }
  }
}

