#pragma once

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Decodificação.h"

using json = nlohmann::json;

class DataLogger {
private:
    std::string dataDirectory;
    std::string currentFilename;
    size_t maxFileSize;
    size_t maxFiles;
    
public:
    DataLogger(const std::string& directory = "/home/pi/ais_data");
    
    bool initialize();
    void logAISData(const AISData& data, const std::string& rawNMEA);
    void logRawNMEA(const std::string& nmeaSentence);
    std::string getCurrentTimestamp();
    std::string getCurrentDate();
    void rotateFilesIfNeeded();
    
    // Getters e Setters
    void setDataDirectory(const std::string& directory);
    void setMaxFileSize(size_t size);
    void setMaxFiles(size_t files);
    
private:
    std::string generateFilename(const std::string& prefix, const std::string& extension = "json");
    json convertAISDataToJSON(const AISData& data, const std::string& rawNMEA);
    bool ensureDirectoryExists(const std::string& path);
};