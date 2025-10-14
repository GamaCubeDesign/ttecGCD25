#include "Integration.h"
#include "Moden.h"


#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;



struct healthData{
    float voltage;
    float current;
    float power;
    float shunt;
    float temperature;
    float pressure;
    float altitude;
    float temperatureD;
    uint8_t length;
};

struct imagingData{
    uint8_t length;
};

healthData health;
imagingData imaging;

void parseHealth(){
    std::ifstream file("sensor_data.json");
    json j;
    file >> j;

    health.voltage = j["sensors"]["INA219"].value("voltage_V", 0.0f);
    health.current = j["sensors"]["INA219"].value("current_mA", 0.0f);
    health.power = j["sensors"]["INA219"].value("power_mW", 0.0f);
    health.shunt = j["sensors"]["INA219"].value("shunt_voltage_mV", 0.0f);
    health.temperature = j["sensors"]["BMP180"].value("temperature_C", 0.0f);
    health.pressure = j["sensors"]["BMP180"].value("pressure_hPa", 0.0f);
    health.altitude = j["sensors"]["BMP180"].value("altitude_m", 0.0f);
    health.temperatureD = j["sensors"]["DS18B20"].value("temperature_C", 0.0f);

    health.length = sizeof(health);

    std::cout << "Voltage INA219: " << health.voltage << " V\n";
    std::cout << "Currente INA219: " << health.current << " mA\n";
    std::cout << "Power INA219: " << health.power << " mW\n";
    std::cout << "Shunt INA219: " << health.current << " mV\n";

    std::cout << "Temperature BMP180: " << health.temperature << " °C\n";
    std::cout << "Pressure BMP180: " << health.pressure << " hPa\n";
    std::cout << "Altitude BMP180: " << health.altitude << " m\n";
    
    std::cout << "Temperature DS18B20: " << health.temperatureD << " °C\n";
   

}


void sendHealthData(){
    tx_send((uint8_t*)&health, health.length);
}


void parseImaging(){
    imaging.length = sizeof(imaging);
}


void SendImagingData(){
    tx_send((uint8_t*)&imaging, imaging.length);
}

int main(){

    parseHealth();
    return 0;
}
