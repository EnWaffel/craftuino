#pragma once

#include "Command.h"

#include <unordered_map>
#include <string>
#include <fstream>

class CommandManager
{
private:
    std::unordered_map<std::string, Command*> commands;
public:
    ~CommandManager();

    void AddCommand(const std::string& name, Command* processer);
    int CheckLine(const std::string& line, int lineNum);
    int CompileLine(Program& prog, const std::string& line, int lineNum, bool isSetup);
    void GenerateCommand(Program& prog, Cmd& cmd, std::ofstream& out);
};