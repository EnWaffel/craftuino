#include "Config.h"
#include "Logger.h"
#include "spdlog/pattern_formatter.h"

#include <inicpp.h>
#include <fstream>

std::string Config::avrdudePath;
std::string Config::avrdudeConfigPath;
std::string Config::arduinoCompilerPath = "arduino-cli";

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

    return 0;
}