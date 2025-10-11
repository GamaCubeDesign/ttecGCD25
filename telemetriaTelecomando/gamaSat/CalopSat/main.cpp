#include <iostream>
#include <thread>

#ifdef __cplusplus
extern "C" {
#include "sx1278-LoRa-RaspberryPi/LoRa.h"
}
#endif


using namespace std;


#include "Moden.h"
#include "Module.h"

bool run = false;

void loop(){
    updateRFComm();
}


int main(int argc, char *argv[]){
    std::cout << "Raspberry Gama Satellite communication system with LoRa Ra-02\n";
    return 0;

    initRFModule();

    std::cout << "Device initiated successfully\n";

    run = true;

    while(run){
        loop();
    }


    return 0;
}