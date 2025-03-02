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
    int CompileLine(Program& prog, const std::string& line, int lineNum, const std::string& func, bool isSubCmd = false, Cmd* subCmd = nullptr);
    void GenerateCommand(Program& prog, Func& func, Cmd& cmd, std::ofstream& out);
};