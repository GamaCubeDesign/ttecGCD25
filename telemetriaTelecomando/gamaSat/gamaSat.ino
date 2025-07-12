#include <SPI.h>
#include <LoRa.h>
#include "GamaSatComm.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(128, 64, &Wire, -1);


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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();         
  display.setTextSize(1);         
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0); 
  
  
  while (!Serial);  

  Serial.println("Testando o sistema de comunicação do Gama Sat com a  Estação Terrestre com o módulo RF LoRa Ra-01");
  display.clearDisplay();
  display.println("** Testando o sistema de comunicacao do Gama Sat com a  Estacao Terrestre com o modulo RF LoRa Ra-01");
  display.display();
  
  SPI.begin(LORA_PIN_SCK, LORA_PIN_MISO, LORA_PIN_MOSI, LORA_PIN_NSS);

  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);

  if (!LoRa.begin(433E6)) {             
    Serial.println("Falha na inicialização do LoRa. Verifique suas conexões.");
    display.println("** Falha na inicializacao do LoRa. Verifique suas conexoes.");
    display.display();
    while (true);                       
  }
  Serial.println("Dispositivo loRa iniciado com sucesso.");
  display.println("** Dispositvo loRa iniciado com succeso.");
  display.display();

  delay(5000);
}



void loop() {
  int packetSize = LoRa.parsePacket();
  char mensagem[256]; 

  if (packetSize) {
    Serial.print("Recebendo e lendo pacote loRa");
    display.clearDisplay();
    display.setCursor(0, 0); 
    display.println("- Recebendo e lendo pacote loRa");
    display.display();
    
    uint8_t txH = LoRa.read();
    uint8_t txL = LoRa.read();
    uint8_t rxH = LoRa.read();
    uint8_t rxL = LoRa.read();
    uint8_t length = LoRa.read();

  if (rxH != rxHSAT || rxL != rxLSAT) {
      Serial.println("Pacote não foi destinado para o Gama Sat");
      display.println("- Pacote nao foi destinado para o Gama Sat");
      display.display();
      while (LoRa.available()) LoRa.read();
      return;
    }

    
  if (length != packetSize - 6) {
    Serial.println("Tamanho do pacote não confere");
    display.println("- Tamanho do pacote não confere");
    display.display();
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
    Serial.println("Check inválido");
    display.println("- Check inválido");
    display.display();
    return;
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

    display.clearDisplay();
    display.setCursor(0, 0); 
    display.println("Pacote recebido com sucesso:");
    display.print("packetsize: ");
    display.print(packetSize);
    display.println(" bytes");
    display.print("   Payload: ");
    display.print(length);
    display.println(" bytes");
    display.print("RSSI: ");
    display.println(LoRa.packetRssi());
    display.print("Mensagem recebida: ");
    display.print(mensagem);
    display.display();
  }

   if (strcmp(mensagem, "GSGAMA:hi, GamaSat") == 0) {
    iniciarComunicacaoComGroundStation();
  }
  
  }

