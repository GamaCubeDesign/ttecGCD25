#include <iostream>
#include <thread>
#include <unistd.h>

#include "Moden.h"
#include "Module.h"
#include "Integration.h"

/*#ifdef __cplusplus
extern "C" {
#include "sx1278-LoRa-RaspberryPi/LoRa.h"
}
#endif
*/

using namespace std;



bool run = false;

void loop(){
    //updateRFComm();
}


int main(int argc, char *argv[]){
    std::cout << "Telemetry and remote control system with LoRa RA-02 using Calopsat's Raspberry Pi\n";

    std::cout << "\nTrying to initialize the LoRa module\n";

    //initRFModule();

    std::cout << "\nDevice initiated successfully\n";

    std::cout << "\nEntering the loop...\n";

    sleep(3);
    
    run = true;

    /*while(run){
        loop();
    }*/
    extern healthData health;  

    healthData removed;
    generateHealthData();
    fila* f = criaFila();

    mostraFIFO(f);                  
    f = enfileira(f, health);
    mostraFIFO(f);                  

    parse2Health();
    f = enfileira(f, health);

    mostraFIFO(f);                  

    desinfileira(f, &removed);

    mostraFIFO(f);                  
    free(f);



    return 0;
}