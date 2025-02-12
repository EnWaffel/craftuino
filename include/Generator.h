#pragma once

#include "Program.h"
#include "command/CommandManager.h"

#include <fstream>

class Generator
{
private:
    std::string sketchFolder;
    CommandManager* cmdMgr;
public:
    Generator(const std::string& sketchFolder, CommandManager& cmdMgr);

    void GenerateCXXCode(Program& prog);
};