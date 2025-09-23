#include <iostream>
using namespace std;

#include "rfmodem.h"


int main(int argc, char *argv[]){
    std::cout << "Raspberry Gama Satellite communication system with LoRa Ra-02\n";
    return 0;

    initRfModule();

    std::cout << "Device initiated successfully\n";


}