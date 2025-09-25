#include <iostream>
#include <thread>

//#ifdef __cplusplus
//extern "C" {
///#include "sx1278-LoRa-RaspberryPi/LoRa.h"
//
//#endif


using namespace std;

//#include "rfmodem.h"
#include "StatusServer.cpp"


StatusFIFO status_fifo;


StatusDataServer status_server(&status_fifo, 8081);

void run_status_server() {
    status_server.run_server();
}


int main(int argc, char *argv[]){
    std::cout << "Raspberry Gama Satellite communication system with LoRa Ra-02\n";
    return 0;

    //initRfModule();

    std::cout << "Device initiated successfully\n";

    thread status_thread(run_status_server);
    status_thread.join();

    return 0;
}