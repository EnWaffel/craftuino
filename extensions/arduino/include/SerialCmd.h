#pragma once

#include "Command.h"

class SerialCmd : public Command
{
public:
    int CheckSyntax(const std::vector<std::string>& args) override;
    std::pair<int, Cmd> Compile(Program& prog, Func& func, const std::vector<std::string>& args) override;
    void Generate(Program& prog, Func& func, Cmd& cmd, std::ofstream& out) override;
};