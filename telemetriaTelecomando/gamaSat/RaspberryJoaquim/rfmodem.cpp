#include "rfmodem.h"



void initRfModule(){
    // See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0; // Raspberry SPI CE pin number
    modem.tx.callback = tx_f;
    modem.rx.callback = rx_f;
    modem.eth.preambleLen = 6;
    modem.eth.bw = BW62_5;            // Bandwidth 62.5KHz
    modem.eth.sf = SF12;              // Spreading Factor 12
    modem.eth.ecr = CR8;              // Error coding rate CR4/8
    modem.eth.CRC = 1;                // Turn on CRC checking
    modem.eth.freq = 433000000;       // 433.0MHz
    modem.eth.resetGpioN = 4;         // GPIO4 on lora RESET pin
    modem.eth.dio0GpioN = 17;         // GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;        // Output power
    modem.eth.powerOutPin = PA_BOOST; // Power Amplifire pin
    modem.eth.AGC = 1;                // Auto Gain Control
    modem.eth.OCP = 240;              // 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;     // Explicit header mode
    modem.eth.syncWord = 0x12;
    // For detail information about SF, Error Coding Rate, Explicit header, Bandwidth,AGC, Over current protection and other features refer to sx127x datasheet https : // www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf
    LoRa_begin(&modem);
    LoRa_receive(&modem);
}
