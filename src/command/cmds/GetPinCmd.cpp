#include "command/cmds/GetPinCmd.h"

int GetPinCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("GetPinCmd: Usage: /getpin [pin(int, var)] [variable(var)] (optional)['DIGITAL', 'ANALOG' (option)]");
        return 1;
    }

    if (args.size() > 2)
    {
        if (args[2] != "DIGITAL" && args[2] != "ANALOG")
        {
            spdlog::error("GetPinCmd: ARG[2] should be an option ('DIGITAL' or 'ANALOG')");
            spdlog::error("GetPinCmd: Usage: /getpin [pin(int, var)] [variable(var)] (optional)['DIGITAL', 'ANALOG' (option)]");
            return 1;
        }
    }

    return 0;
}

std::pair<int, Cmd> GetPinCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "getpin";
    cmd.args = args;

    return { 0, cmd };
}

void GetPinCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args.size() < 3)
    {
        out << cmd.args[1];
        out << '=';
        out << "digitalRead(";
        out << cmd.args[0];
        out << ");";
        return;
    }

    if (cmd.args[2] == "DIGITAL")
    {
        out << cmd.args[1];
        out << '=';
        out << "digitalRead(";
        out << cmd.args[0];
        out << ");";
    }
    else if (cmd.args[2] == "ANALOG")
    {
        out << cmd.args[1];
        out << '=';
        out << "analogRead(";
        out << cmd.args[0];
        out << ");";
    }
}
