#pragma once

#include <string>

class Config
{
public:
    static std::string avrdudePath;
    static std::string avrdudeConfigPath;
    static std::string arduinoCompilerPath;
public:
    static int Load(const std::string configPath);
};