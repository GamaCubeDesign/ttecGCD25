#include "Dados.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

namespace fs = std::filesystem;

DataLogger::DataLogger(const std::string& directory)
    : dataDirectory(directory), maxFileSize(10 * 1024 * 1024), maxFiles(50) {
}

bool DataLogger::initialize() {
    if (!ensureDirectoryExists(dataDirectory)) {
        std::cerr << "ERRO: Não foi possível criar diretório de dados: " << dataDirectory << std::endl;
        return false;
    }

    // subdiretórios
    ensureDirectoryExists(dataDirectory + "/raw");
    ensureDirectoryExists(dataDirectory + "/processed");
    ensureDirectoryExists(dataDirectory + "/vessels");
    ensureDirectoryExists(dataDirectory + "/daily");

    std::cout << "DataLogger inicializado. Diretório: " << dataDirectory << std::endl;
    return true;
}

void DataLogger::logAISData(const AISData& data, const std::string& rawNMEA) {
    if (!data.isValid) return;

    try {
        // Principal
        std::string mainFile = dataDirectory + "/ais_data.json";
        json mainData;

        // Ler dados existentes
        std::ifstream inputFile(mainFile);
        if (inputFile.good()) {
            inputFile >> mainData;
        } else {
            mainData = {
                {"metadata", {
                    {"description", "Dados AIS coletados"},
                    {"version", "1.0"},
                    {"created", getCurrentTimestamp()}
                }},
                {"vessels", json::object()},
                {"messages", json::array()}
            };
        }
        inputFile.close();

        // Nova mensagem
        json messageJson = convertAISDataToJSON(data, rawNMEA);
        mainData["messages"].push_back(messageJson);

        // Embarcação
        std::string mmsiStr = std::to_string(data.mmsi);
        if (!mainData["vessels"].contains(mmsiStr)) {
            mainData["vessels"][mmsiStr] = json::object();
        }

        json& vessel = mainData["vessels"][mmsiStr];
        vessel["mmsi"] = data.mmsi;
        vessel["last_seen"] = getCurrentTimestamp();

        if (!data.vesselName.empty() && data.vesselName != "Unknown")
            vessel["name"] = data.vesselName;
        if (!data.callSign.empty() && data.callSign != "Unknown")
            vessel["callsign"] = data.callSign;

        // Atualiza posição
        if (data.latitude != 0.0 && data.longitude != 0.0) {
            vessel["last_position"] = {
                {"latitude", data.latitude},
                {"longitude", data.longitude},
                {"timestamp", getCurrentTimestamp()}
            };

            // Histórico das posições
            if (!vessel.contains("position_history"))
                vessel["position_history"] = json::array();

            json positionRecord = {
                {"latitude", data.latitude},
                {"longitude", data.longitude},
                {"speed", data.speedOverGround},
                {"course", data.courseOverGround},
                {"heading", data.trueHeading},
                {"timestamp", getCurrentTimestamp()}
            };

            vessel["position_history"].push_back(positionRecord);

            // Mantem últimas 10 posições
            if (vessel["position_history"].size() > 10) {
                json newArray = json::array();
                auto& posHist = vessel["position_history"];
                for (size_t i = posHist.size() - 10; i < posHist.size(); ++i) {
                    newArray.push_back(posHist[i]);
                }
                vessel["position_history"] = newArray;
            }
        }

        // Salvar principal
        std::ofstream outputFile(mainFile);
        outputFile << mainData.dump(4);
        outputFile.close();

        // Historico  
        std::string dailyFile = dataDirectory + "/daily/ais_" + getCurrentDate() + ".json";
        json dailyData;

        std::ifstream dailyInput(dailyFile);
        if (dailyInput.good()) {
            dailyInput >> dailyData;
        } else {
            dailyData = {
                {"date", getCurrentDate()},
                {"messages", json::array()}
            };
        }
        dailyInput.close();

        dailyData["messages"].push_back(messageJson);

        std::ofstream dailyOutput(dailyFile);
        dailyOutput << dailyData.dump(4);
        dailyOutput.close();

        // arquivo por embarcação
        std::string vesselFile = dataDirectory + "/vessels/" + mmsiStr + ".json";
        json vesselData;

        std::ifstream vesselInput(vesselFile);
        if (vesselInput.good()) {
            vesselInput >> vesselData;
        } else {
            vesselData = {
                {"mmsi", data.mmsi},
                {"messages", json::array()}
            };
        }
        vesselInput.close();

        vesselData["messages"].push_back(messageJson);
        vesselData["last_update"] = getCurrentTimestamp();

        std::ofstream vesselOutput(vesselFile);
        vesselOutput << vesselData.dump(4);
        vesselOutput.close();

    } catch (const std::exception& e) {
        std::cerr << "Erro ao salvar dados JSON: " << e.what() << std::endl;
    }
}

void DataLogger::logRawNMEA(const std::string& nmeaSentence) {
    if (nmeaSentence.empty()) return;

    try {
        std::string rawFile = dataDirectory + "/raw/ais_raw_" + getCurrentDate() + ".log";
        std::ofstream file(rawFile, std::ios::app);
        if (file.good()) {
            file << getCurrentTimestamp() << "," << nmeaSentence << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao salvar NMEA raw: " << e.what() << std::endl;
    }
}

json DataLogger::convertAISDataToJSON(const AISData& data, const std::string& rawNMEA) {
    json messageJson = {
        {"timestamp", getCurrentTimestamp()},
        {"raw_nmea", rawNMEA},
        {"mmsi", data.mmsi},
        {"message_type", static_cast<int>(data.messageType)}
    };

    std::string messageTypeStr;
    switch (data.messageType) {
        case AISData::MessageType::POSITION_CLASS_A:  messageTypeStr = "POSITION_CLASS_A"; break;
        case AISData::MessageType::POSITION_CLASS_A_2: messageTypeStr = "POSITION_CLASS_A_2"; break;
        case AISData::MessageType::POSITION_CLASS_A_3: messageTypeStr = "POSITION_CLASS_A_3"; break;
        case AISData::MessageType::BASE_STATION:      messageTypeStr = "BASE_STATION"; break;
        case AISData::MessageType::STATIC_DATA:       messageTypeStr = "STATIC_DATA"; break;
        case AISData::MessageType::POSITION_CLASS_B:  messageTypeStr = "POSITION_CLASS_B"; break;
        default:                                      messageTypeStr = "UNKNOWN";
    }
    messageJson["message_type_str"] = messageTypeStr;

    if (data.latitude != 0.0 || data.longitude != 0.0)
        messageJson["position"] = {{"latitude", data.latitude}, {"longitude", data.longitude}};

    if (data.speedOverGround != 0.0)
        messageJson["speed_over_ground"] = data.speedOverGround;

    if (data.courseOverGround != 0.0)
        messageJson["course_over_ground"] = data.courseOverGround;

    if (data.trueHeading != 0)
        messageJson["true_heading"] = data.trueHeading;

    if (!data.vesselName.empty() && data.vesselName != "Unknown")
        messageJson["vessel_name"] = data.vesselName;

    if (!data.callSign.empty() && data.callSign != "Unknown")
        messageJson["callsign"] = data.callSign;

    return messageJson;
}

std::string DataLogger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string DataLogger::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d");
    return ss.str();
}

bool DataLogger::ensureDirectoryExists(const std::string& path) {
    try {
        if (!fs::exists(path)) {
            fs::create_directories(path);
        }
        return fs::is_directory(path);
    } catch (const std::exception& e) {
        std::cerr << "Erro ao criar diretório: " << path << " -> " << e.what() << std::endl;
        return false;
    }
}

void DataLogger::setDataDirectory(const std::string& directory) {
    dataDirectory = directory;
}

void DataLogger::setMaxFileSize(size_t size) {
    maxFileSize = size;
}

void DataLogger::setMaxFiles(size_t files) {
    maxFiles = files;
}

std::string DataLogger::generateFilename(const std::string& prefix, const std::string& extension) {
    return dataDirectory + "/" + prefix + "_" + getCurrentTimestamp() + "." + extension;
}

void DataLogger::rotateFilesIfNeeded() {
    // Implementar rotação futura se necessário
}
