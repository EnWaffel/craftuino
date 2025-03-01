#pragma once

#include <string>
#include <cstdint>
#include <vector>

struct WorkStep
{
    std::string prog;
    std::string args;   
};

class Config
{
public:
    static std::string board;
    static std::string programmer;
    static std::string chip;
    static std::string baudRate;
    static std::string port;
    static std::string sketchDir;
    static std::string sketchName;
    static int compileStepsCount;
    static int flashStepsCount;
    static std::vector<WorkStep> compileSteps;
    static std::vector<WorkStep> flashSteps;
    static bool verbose;
public:
    static int Load(const std::string configPath);
    static std::string ProcessPlaceholders(const std::string& in);
};