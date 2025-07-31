#include <SPI.h>
#include <LoRa.h>
#include "GamaSatComm.h"





#define LORA_PIN_SCK      18     // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     23    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      5    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes))

#define rxHSAT  0x02
#define rxLSAT  0x20


void setup() {
  Serial.begin(115200);
  
  while (!Serial);  

  Serial.println("Testing the Gama Sat communication system with the Earth Station with the LoRa Ra-02 RF module.");
  
  SPI.begin(LORA_PIN_SCK, LORA_PIN_MISO, LORA_PIN_MOSI, LORA_PIN_NSS);

  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);

  if (!LoRa.begin(433E6)) {             
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);                       
  }
  Serial.println("LoRa device started successfully.");

  delay(5000);
}



void loop() {
  Serial.println("waiting for telemetry");
  int packetSize = LoRa.parsePacket();
  char mensagem[256]; 

  if (packetSize) {
    Serial.print("Recebendo e lendo pacote loRa");
    
    uint8_t txH = LoRa.read();
    uint8_t txL = LoRa.read();
    uint8_t rxH = LoRa.read();
    uint8_t rxL = LoRa.read();
    uint8_t length = LoRa.read();

  if (rxH != rxHSAT || rxL != rxLSAT) {
      Serial.println("Package was not intended for Gama GroundStation");
      while (LoRa.available()) LoRa.read();
      return;
    }

    
  if (length != packetSize - 6) {
    Serial.println("Incorrect package size");
    while (LoRa.available()) LoRa.read();
    return;
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
    return;
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
    Serial.print("SNR: ");
    Serial.println(LoRa.packetSnr());
    Serial.print("Message received: ");
    Serial.println(mensagem);

  }

   if (strcmp(mensagem, "GSGAMA:hi, GamaSat") == 0) {
      iniciarComunicacaoComGroundStation();
  }else if (strcmp(mensagem, "hd") == 0) {
      Serial.println("requested health data");
      hdata();
    }
  
  }

