#include "command/cmds/SetCmd.h"

int SetCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("SetCmd: Usage: /set [name(string)] [? (literal, var)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> SetCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "set";
    cmd.args = args;

    Var* v = GetVarByName(prog, func, args[0]);
    if (!v)
    {
        spdlog::error("SetCmd: Var: '{0}' is out of scope or doesn't exist", args[0]);
        return { 1, cmd };
    }

    if (v->type != EstimateVarTypeByValue(args[1]))
    {
        spdlog::error("SetCmd: Var: '{0}' and Value: '{1}' are not the same type", args[0], args[1]);
        return { 1, cmd };
    }

    return { 0, cmd };
}

void SetCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    out << cmd.args[0];
    out << '=';
    out << cmd.args[1];
    out << ';';
}
