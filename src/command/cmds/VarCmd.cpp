#include "command/cmds/VarCmd.h"

int VarCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("VarCmd: Usage: /var [name(string)] [? (literal)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> VarCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "var";
    cmd.args = args;
    cmd.args.push_back(args[1]);
    cmd.isVar = true;

    return { 0, cmd };
}

void VarCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    out << VarTypeToStr(EstimateVarTypeByValue(cmd.args[1]));
    SPACE;
    out << cmd.args[0];
    out << '=';
    out << cmd.args[1];
    out << ';';
}
