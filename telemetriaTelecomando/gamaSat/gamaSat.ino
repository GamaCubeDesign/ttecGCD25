#include <SPI.h>
#include <LoRa.h>
#include "GamaSatComm.h"


#define LORA_PIN_SCK       5    // Clock SPI do LoRa
#define LORA_PIN_MISO     19    // MISO (Master In Slave Out)
#define LORA_PIN_MOSI     27    // MOSI (Master Out Slave In)
#define LORA_PIN_NSS      18    // Chip Select (às vezes chamado CS ou SS)
#define LORA_PIN_RESET    14    // Pino de reset do módulo LoRa
#define LORA_PIN_IRQ      26    // Pino de interrupção DIO0 (recepção de pacotes)

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Aguarda o usuário abrir o Monitor Serial

  Serial.println("Testando o sistema de comunicação do Gama Sat com a  Estação Terrestre com o módulo RF LoRa Ra-01");

  // substituir os pinos CS, reset e IRQ padrão (opcional)
  LoRa.setPins(LORA_PIN_NSS, LORA_PIN_RESET, LORA_PIN_IRQ);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // Inicialia o radio em 433mhz
    Serial.println("Falha na inicialização do LoRa. Verifique suas conexões.");
    while (true);                       // se falhar, não faz nada
  }
  Serial.println("Dispositivo iniciado com sucesso.");
}



void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String recebido = "";
    while (LoRa.available()) {
      recebido += (char)LoRa.read();
    }

    int separador = recebido.indexOf(':');
    if (separador != -1) {
      String remetente = recebido.substring(0, separador);
      String mensagem = recebido.substring(separador + 1);

      if (remetente == "GSGAMA") {
        Serial.print("[De ");
        Serial.print(remetente);
        Serial.print("] ");
        Serial.println(mensagem);
        if (mensagem == "Olá, GamaSat"){
            iniciarComunicacaoComGroundStation();
        }
      } else {
        Serial.print("Mensagem ignorada de ");
        Serial.println(remetente);
      }
    } else {
      Serial.println("Pacote inválido (sem cabeçalho)");
    }

    //Serial.print("RSSI: ");
    //Serial.println(LoRa.packetRssi());
  }
}
