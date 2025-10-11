#include <iostream>
#include <thread>
#include <filesystem>
#include <sys/statvfs.h>
#include <cstring>
using namespace std;

#ifdef __cplusplus
extern "C" {
#include "sx1278-LoRa-RaspberryPi/LoRa.h"
}
#endif

#include "RFModem.h"
#include "Logger.h"
#include "RFModule.h"
#include "Timing.h"
#include "StatusServer.cpp"
#include "ImagingServer.cpp"
#include "printing_utils.cpp"

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;
bool enable_writing = false;

ImagingFIFO imaging_fifo;
StatusFIFO status_fifo;

StatusDataServer status_server(&status_fifo, 8081);
ImagingDataServer imaging_server(&imaging_fifo, 8080);

bool running = false;

void run_status_server() {
    status_server.run_server();
}

void run_imaging_server() {
    imaging_server.run_server();
}

void read_fifos() {
    while (imaging_fifo.available()) {
        ImagingData imagingPacket = imaging_fifo.read();
        logger.writeSatImagingDataPacket(imagingPacket);
    }

    while (status_fifo.available()) {
        HealthData statusPacket = status_fifo.read();
        logger.writeSatStatusPacket(statusPacket);
    }
}

void checkConsole() {
    char c;
    while (console.available() > 0) {
        c = console.read();
        GSPacket sending_packet;

        switch (c) {
            case '0':
                std::cout << "Enabling writing" << std::endl;
                enable_writing = !enable_writing;
                break;

            case '1':
                std::cout << "Switching status file" << std::endl;
                logger.switch_status_file();
                break;

            case '2':
                std::cout << "Switching imaging data file" << std::endl;
                logger.switch_imaging_file();
                break;

            case '3':
                std::cout << "Sending status request" << std::endl;
                sending_packet.operation.protocol = PROTOCOL_STATUS;
                sending_packet.operation.operation = GS_STATUS_REQUEST;
                sending_packet.length = 2;
                modem_write((uint8_t*)&sending_packet, sending_packet.length);
                break;

            case '4':
                std::cout << "Sending status start transmission" << std::endl;
                sending_packet.operation.protocol = PROTOCOL_STATUS;
                break;

            case 'b':
                std::cout << "Echo console" << std::endl;
                break;
        }
    }
}

void loop() {
    checkConsole();
    updateRFComm();
    read_fifos();
}

int main(int argc, char *argv[]) {
    std::cout << "Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

    initRFModule();
    std::cout << "Device initiated successfully\n";

    std::thread status_thread(run_status_server);
    std::thread imaging_thread(run_imaging_server);

    running = true;

    while (running) {
        loop();
    }

    cout << "Closing servers" << endl;
    status_server.close_server();
    imaging_server.close_server();

    status_thread.join();
    imaging_thread.join();

    exit(1);
    modem_finish();
    return 0;
}
