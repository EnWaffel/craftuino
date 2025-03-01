#include "Config.h"
#include "Logger.h"
#include "spdlog/pattern_formatter.h"
#include "Utils.h"

#include <inicpp.h>
#include <fstream>

std::string Config::board;
std::string Config::programmer;
std::string Config::chip;
std::string Config::baudRate;
std::string Config::port;
std::string Config::sketchDir;
std::string Config::sketchName;
bool Config::verbose = false;
int Config::compileStepsCount = 0;
int Config::flashStepsCount = 0;
std::vector<WorkStep> Config::compileSteps;
std::vector<WorkStep> Config::flashSteps;

int Config::Load(const std::string configPath)
{
    std::ifstream s(configPath);
    if (!s.is_open())
    {
        s.open("configs/" + configPath);
        if (!s.is_open())
        {
            spdlog::error("Config: Couldn't access '" + configPath + "'");
            return 1;
        }
    }

    ini::IniFile cfg;
    cfg.decode(s);

    compileStepsCount = cfg["steps"]["compile"].as<int>();
    flashStepsCount = cfg["steps"]["flash"].as<int>();

    if (compileStepsCount < 1 || flashStepsCount < 1)
    {
        spdlog::error("Config: Need at least 1 compile and flash step!");
        return 1;
    }

    for (int i = 0; i < compileStepsCount; i++)
    {
        std::string name = "compile-" + std::to_string(i + 1);
        if (cfg.find(name) == cfg.end()) continue;

        WorkStep step{};
        step.prog = cfg[name]["prog"].as<std::string>();
        step.args = cfg[name]["args"].as<std::string>();

        compileSteps.push_back(step);
    }

    for (int i = 0; i < flashStepsCount; i++)
    {
        std::string name = "flash-" + std::to_string(i + 1);
        if (cfg.find(name) == cfg.end()) continue;

        WorkStep step{};
        step.prog = cfg[name]["prog"].as<std::string>();
        step.args = cfg[name]["args"].as<std::string>();

        flashSteps.push_back(step);
    }

    board = cfg["sketch"]["board"].as<std::string>();
    programmer = cfg["sketch"]["programmer"].as<std::string>();
    chip = cfg["sketch"]["chip"].as<std::string>();
    baudRate = cfg["sketch"]["baud"].as<std::string>();
    port = cfg["sketch"]["port"].as<std::string>();

    return 0;
}

std::string Config::ProcessPlaceholders(const std::string& in)
{
    std::string result = in;

    result = ReplaceString(result, "%board%", board);
    result = ReplaceString(result, "%programmer%", programmer);
    result = ReplaceString(result, "%chip%", chip);
    result = ReplaceString(result, "%baud%", baudRate);
    result = ReplaceString(result, "%port%", port);
    result = ReplaceString(result, "%buildpath%", sketchDir + "/build/bin");
    result = ReplaceString(result, "%genpath%", sketchDir + "/build/gen");
    result = ReplaceString(result, "%sketch%", sketchName);
    result = ReplaceString(result, "%sketchpath%", sketchDir);

    return result;
}
