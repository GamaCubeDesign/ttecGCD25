#pragma once

#include <string>
#include <cstdint>

struct NMEAData {
    std::string rawSentence;
    std::string sentenceType;
    std::string encodedData;
    int fragmentNumber;
    int totalFragments;
    char channel;
    bool isValid;
    
    NMEAData() : fragmentNumber(0), totalFragments(0), channel('A'), isValid(false) {}
};

struct AISData {
    uint32_t mmsi;
    enum class MessageType {
        UNKNOWN = 0,
        POSITION_CLASS_A = 1,
        POSITION_CLASS_A_2 = 2,
        POSITION_CLASS_A_3 = 3,
        BASE_STATION = 4,
        STATIC_DATA = 5,
        POSITION_CLASS_B = 18
    } messageType;
    
    double latitude;
    double longitude;
    double speedOverGround;
    double courseOverGround;
    int trueHeading;
    std::string vesselName;
    std::string callSign;
    
    bool isValid;
    
    AISData() : mmsi(0), messageType(MessageType::UNKNOWN), 
                latitude(0.0), longitude(0.0), 
                speedOverGround(0.0), courseOverGround(0.0),
                trueHeading(0), isValid(false) {}
};

class AISParser {
public:
    NMEAData parseNMEA(const std::string& nmeaSentence);
    AISData decodeAIS(const NMEAData& nmeaData);
    
private:
    uint8_t decodeAISChar(char c);
    int decodeAISInteger(const std::string& data, int startBit, int length);
    std::string decodeAISString(const std::string& data, int startBit, int length);
    double decodeAISLatLong(const std::string& data, int startBit, int length);
};