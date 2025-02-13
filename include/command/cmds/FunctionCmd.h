#pragma once

#include "../Command.h"

class FunctionCmd : public Command
{
public:
    int CheckSyntax(const std::vector<std::string>& args) override;
    std::pair<int, Cmd> Compile(const std::vector<std::string>& args) override;
    void Generate(Program& prog, Cmd& cmd, std::ofstream& out) override;
};