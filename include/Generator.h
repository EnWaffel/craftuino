#pragma once

#include "Program.h"
#include "command/CommandManager.h"

#include <fstream>
#include <string>

class Generator
{
private:
    std::string sketchFolder;
    CommandManager* cmdMgr = nullptr;
    std::string sketchName;
    std::string sketchOutPath;
    float numToGenerate = 0.0f;
    float numGenerated = 0.0f;
public:
    Generator(const std::string& sketchFolder, CommandManager& cmdMgr);

    int GenerateCXXCode(Program& prog);
private:
    int GenIno(Program& prog);
    int GenFile(Program& prog, const std::string& name);
    float GetGenPercent();
    void LogGenNext(const std::string& path);
};