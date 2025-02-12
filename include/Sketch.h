#pragma once

#include "command/CommandManager.h"
#include "Program.h"

#include <string>

class Sketch
{
private:
    std::string sketchFolder;
    CommandManager cmdMgr;
    Program prog;
public:
    Sketch();
    ~Sketch();

    int DoDir(const std::string folderPath);
    int Compile(std::ifstream& in, bool isSetup);
    int Upload();
private:
    int Parse(std::ifstream& in);
};