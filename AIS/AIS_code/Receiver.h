#pragma once

#include <string>
#include <wiringSerial.h>

class AISReceiver {
private:
    std::string serialPort;
    int baudRate;
    int serialHandle;
    
public:
    AISReceiver(const std::string& port = "/dev/ttyS0", int baud = 38400);
    ~AISReceiver();
    
    bool initialize();
    std::string readMessage();
    void sendCommand(const std::string& command);
    
public:
    bool validateChecksum(const std::string& nmeaSentence);
};