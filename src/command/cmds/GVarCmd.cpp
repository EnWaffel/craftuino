#include "command/cmds/GVarCmd.h"

int GVarCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("GVarCmd: Usage: /gvar [name(string)] [? (literal)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> GVarCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "gvar";
    cmd.args = args;
    cmd.args.push_back(args[1]);
    cmd.isVar = true;

    return { 0, cmd };
}

void GVarCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
}
