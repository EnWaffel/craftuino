#include "command/cmds/FunctionCmd.h"

int FunctionCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 1)
    {
        spdlog::error("PinModeCmd: Usage: /function [functionName(string)] [...(variables / literals)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> FunctionCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "function";
    cmd.args = args;

    return { 0, cmd };
}

void FunctionCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    out << cmd.args[0] << '(';

    for (int i = 1; i < cmd.args.size(); i++)
    {
        out << cmd.args[i];
    }

    out << ");";
}
