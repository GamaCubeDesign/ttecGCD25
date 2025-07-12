<p align="center">
  <img src="https://github.com/GamaCubeDesign/ttecGCD25/blob/main/interface/resources/gamaY.png?raw=true" alt="image alt" width="50%">
</p>


Esse repositorio é destinado a equipe de ttec da Gama Cube Desing para a CubeDesing de 2025.

Uma parte desse projeto tem como objetivo decodificar mensagens AIS (Automatic Identification System) do tipo 1, que são amplamente utilizadas para transmitir informações de posição e navegação de embarcações.

Outra parte é destinada ao sistema de telemetria e telecomando.

O diretório "Docs" contém a documentação do projeto:
- O diretório "AISDoc" contém a documentação sobre o sistema AIS.
- O diretório "groundStationDoc" contém a documentação referente ao sistema de telemetria e telecomando da GroundStation.
- O diretório "gamaSatDoc" contém a documentação referente ao sistema de telemetria e telecomando do Cubesat.


## Sistema AIS:

Os dados AIS serão decodificados em NMEA0183.

Mensagem AIS do tipo 1 (exemplo): !AIVDM,1,1,,A,13aEOK?P00PD2wVMdLDRhgvL289?,0*26.

Parâmetros AIS decodificados necessários para a missão:
- Message ID
- User ID
- Navigational status
- Rate of turn ROTAIS
- SOG
- Longitude
- Latitude
- True heading
- Time stamp


## Organização:

O diretório "AIS" contém tudo relacionado ao sistema AIS.
    - O diretório "aisDecoders" contém os códigos de decodificação AIS.
    - O arquivo "aisExamples.txt" contém mensagens AIS do tipo 1 para testes.



## Sistema de Telemetria e Telecomando:

O sistema de Telemetria e Telecomando é feito a partir da comunicação LoRa, a frequência utilizada é 433mhz, ou seja, é um sistema que utilizada rádio frequência UHF (Ultra High Frequency).

O sistema também conta com criptografia, ... (Ainda em pesquisa!).

- Requisitos do sistema:
    - Testar a inicialização da integração dos sistemas (GroundStation -> GamaSat | GamaSat -> GroundStation).
    - Dados de saúde: Tensão, temperatura externa e interna, corrente, memória da rasp, altitude.
    - Dados da missão: Área de óleo, dados da embarcação que está poluindo.
    - Comunicação em 30 metros (minimo).
    - Escutar a groundstation a cada estante.
    - O sistema deve ser integrado com o sensoriamento remoto.

## Organização:

O diretório "telemetriaTelecomando" contém os arquivos do sistema de Telemetria e Telecomando.
    - o diretório "groundStation" contém os arquivos relacionados a GroundStation.
    - O diretório "gamaSat" contém os arquivos relacionados ao Cubesat.




