#include <SPI.h>
#include <LoRa.h>
#include "GroundStationComm.h"

#define LORA_PIN_SCK      18     // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     23    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      5    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes)

void setup() {
  Serial.begin(115200);
  while (!Serial);  
  delay(1000);
  Serial.println("Testing the Gama Ground Station communication system with the LoRa Ra-02 RF module");

  SPI.begin(LORA_PIN_SCK, LORA_PIN_MISO, LORA_PIN_MOSI, LORA_PIN_NSS);

  // substituir os pinos CS, reset e IRQ padrão (opcional)
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ); // set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {
    Serial.println("ERROR 101");
    while (true);                       
  }

  Serial.println("Device started successfully.");
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
    }
    else if (comand == "") {
      
    }
    else {
      Serial.println("ERROR 301");
    }
  }
}

