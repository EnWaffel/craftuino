#include "Config.h"
#include "Logger.h"
#include "spdlog/pattern_formatter.h"

#include <inicpp.h>
#include <fstream>

std::string Config::avrdudePath;
std::string Config::avrdudeConfigPath;
std::string Config::arduinoCompilerPath = "arduino-cli";
std::string Config::board = "";
std::string Config::programmer = "";
std::string Config::chip = "";
std::string Config::baudRate = "";
std::string Config::port = "";
bool Config::verbose = false;

int Config::Load(const std::string configPath)
{
    std::ifstream s(configPath);
    if (!s.is_open())
    {
        spdlog::error("Config: Couldn't access '" + configPath + "'");
        return 1;
    }

    ini::IniFile cfg;
    cfg.decode(s);

    avrdudePath = cfg["arduino"]["avrdude"].as<std::string>();
    avrdudeConfigPath = cfg["arduino"]["avrdude_config"].as<std::string>();
    arduinoCompilerPath = cfg["arduino"]["compiler"].as<std::string>();
    board = cfg["sketch"]["board"].as<std::string>();
    programmer = cfg["sketch"]["programmer"].as<std::string>();
    chip = cfg["sketch"]["chip"].as<std::string>();
    baudRate = cfg["sketch"]["baud"].as<std::string>();
    port = cfg["sketch"]["port"].as<std::string>();

    return 0;
}