#include <SPI.h>
#include <LoRa.h>
#include "GroundStationComm.h"


#define LORA_PIN_SCK       5    // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     27    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      18    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes)

void Menu() {
  Serial.println("\nDigite uma das opções:");
  Serial.println("1 - Teste de comunicação Ground Station -> GamaSat | GamaSat -> GroundStation");
  Serial.println("2 - opcao");
  Serial.println("3 - opcao");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Aguarda o usuário abrir o Monitor Serial

  Serial.println("Testando o sistema de comunicação da Estação Terrestre Gama com o módulo RF LoRa Ra-01");

  // substituir os pinos CS, reset e IRQ padrão (opcional)
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // Inicialia o radio em 433mhz
    Serial.println("Falha na inicialização do LoRa. Verifique suas conexões.");
    while (true);                       // se falhar, não faz nada
  }
  Serial.println("Dispositivo iniciado com sucesso.");
  Menu();  // Mostra o menu logo no início

}



void loop() {
  if (Serial.available()) {
    delay(200);
    char opcao = Serial.read();
    while (Serial.available()) Serial.read();


    switch (opcao) {
      case '1':
        Serial.println("Esp32 leu o numero 1 do monitor serial!");
        iniciarComunicacaoComSatelite();
        break;

      case '2':
        //chama função de alguma biblioteca.
        break;

      case '3':
        //chama função de alguma biblioteca.
        break;

      default:
        Serial.println("Opção inválida.");
        break;
    }
    Menu();  // Mostra o menu novamente
  }
}
