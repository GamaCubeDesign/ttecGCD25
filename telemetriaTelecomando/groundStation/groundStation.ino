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

  Serial.println("Testando o sistema de comunicação da Estação Terrestre Gama com o módulo RF LoRa Ra-01");

  SPI.begin(LORA_PIN_SCK, LORA_PIN_MISO, LORA_PIN_MOSI, LORA_PIN_NSS);

  // substituir os pinos CS, reset e IRQ padrão (opcional)
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ); // set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {
    Serial.println("Falha na inicialização do LoRa. Verifique suas conexões.");
    while (true);                       
  }

  Serial.println("Dispositivo iniciado com sucesso.");
  Menu();  
}



void loop() {
  if (Serial.available()) {
    String comand = Serial.readStringUntil('\n');
    comand.trim();

    if (comand == "initcomm") {
      iniciarComunicacaoComSatelite();
    }
    else if (comand == "") {
  
    }
    else if (comand == "") {
      
    }
    else {
      Serial.println("ERROR 301");
    }
  }
}

