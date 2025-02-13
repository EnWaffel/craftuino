#pragma once

#include <string>
#include <cstdint>

class Config
{
public:
    static std::string avrdudePath;
    static std::string avrdudeConfigPath;
    static std::string arduinoCompilerPath;
    static std::string board;
    static std::string programmer;
    static std::string chip;
    static std::string baudRate;
    static std::string port;
    static bool verbose;
public:
    static int Load(const std::string configPath);
};