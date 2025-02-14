#pragma once

#include "Program.h"
#include "Logger.h"
#include "Utils.h"

#include <vector>
#include <string>
#include <fstream>

class Command
{
protected:
    Program* prog;
    Func* func;
public:
    Command() = default;
    virtual ~Command() = default;

    virtual int CheckSyntax(const std::vector<std::string>& args) = 0;
    virtual std::pair<int, Cmd> Compile(const std::vector<std::string>& args) = 0;
    virtual void Generate(Program& prog, Cmd& cmd, std::ofstream& out) = 0;

    void SetProgramAndFunc(Program* prog, Func* func);
};