#pragma once

#include "command/CommandManager.h"
#include "Program.h"

#include <string>
#include <fstream>

class Sketch
{
public:
    static Sketch* CUR_SKETCH;
public:
    std::string sketchFolder;
    std::string sketchName;
    CommandManager cmdMgr;
    Program prog;
    std::unordered_map<std::string, std::ifstream*> streams;
    bool onlyGenerate = false;
public:
    Sketch();
    ~Sketch();

    int DoDir(const std::string folderPath);
    int Compile(std::ifstream& in, const std::string& _funcName);
    int Build();
    int Upload();
    std::pair<int, FuncHeader> ReadFunctionHeader(std::ifstream& in, const std::string& func);
private:
    int Parse(std::ifstream& in);
};