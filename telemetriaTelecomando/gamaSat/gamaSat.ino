#include <SPI.h>
#include <LoRa.h>
#include "GamaSatComm.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);



#define LORA_PIN_SCK       5    // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     27    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      18    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes)

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();         
  display.setTextSize(1);         
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0); 
  
  
  while (!Serial);  // Aguarda o usuário abrir o Monitor Serial

  Serial.println("Testando o sistema de comunicação do Gama Sat com a  Estação Terrestre com o módulo RF LoRa Ra-01");
  display.clearDisplay();
  display.println("** Testando o sistema de comunicacao do Gama Sat com a  Estacao Terrestre com o modulo RF LoRa Ra-01");
  display.display();
  
  
        // substituir os pinos CS, reset e IRQ padrão (opcional)
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // Inicialia o radio em 433mhz
    Serial.println("Falha na inicialização do LoRa. Verifique suas conexões.");
    display.println("** Falha na inicializacao do LoRa. Verifique suas conexoes.");
    display.display();
    while (true);                       // se falhar, não faz nada
  }
  Serial.println("Dispositivo loRa iniciado com sucesso.");
  display.println("** Dispositvo loRa iniciado com succeso.");
  display.display();

  delay(5000);
}



void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    uint8_t txH = LoRa.read();
    uint8_t txL = LoRa.read();
    uint8_t rxH = LoRa.read();
    uint8_t rxL = LoRa.read();
    uint8_t length = LoRa.read();

    
    if (length != packetSize - 5) {
      Serial.println("Tamanho do pacote não confere!");
      // Limpar buffer
      while (LoRa.available()) LoRa.read();
      return;
    }

    Serial.print("packetsize: ");
    Serial.print(packetSize);
    Serial.print(" bytes\n");
    Serial.print("Payload: ");
    Serial.print(length);
    Serial.print(" bytes\n");

    display.clearDisplay();
    display.setCursor(0, 0); 
    display.print("packetsize: ");
    display.print(packetSize);
    display.println(" bytes");
    display.print("Payload: ");
    display.print(length);
    display.println(" bytes");
    display.display();
    

    char mensagem[256]; // buffer para mensagem recebida, tamanho max 256 bytes
    for (int i = 0; i < length; i++) {
      mensagem[i] = (char)LoRa.read();
    }
    mensagem[length] = '\0'; 

    Serial.print("Pacote recebido de: 0x");
    Serial.print(txH, HEX);
    Serial.print(txL, HEX);
    Serial.print(" para 0x");
    Serial.print(rxH, HEX);
    Serial.println(rxL, HEX);

    Serial.print("Mensagem: ");
    Serial.println(mensagem);
  }

    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi()); //RSSI mede a intensidade do sinal, isso vai nos ajudar a analisar a qualidade do sinal que estamos enviando.
  }
}
