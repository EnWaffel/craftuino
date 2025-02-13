#include "command/cmds/DelayCmd.h"

int DelayCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("DelayCmd: Usage: /delay [time(int or var)] [ms, u, s(string)]");
        return 1;
    }

    if (args[1] != "ms" && args[1] != "u" && args[1] != "s" )
    {
        spdlog::error("DelayCmd: ARG[2] should be a string ('ms' or 'u' or 's')");
        spdlog::error("DelayCmd: Usage: /delay [time(int or var)] [ms, u, s(string)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> DelayCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "delay";
    cmd.args = args;

    return { 0, cmd };
}

void DelayCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    bool isVar = !IsNumber(cmd.args[0]);

    if (cmd.args[1] == "s")
    {
        out << "delay(";
        if (isVar)
        {
            out << cmd.args[0];
        }
        else
        {
            out << atoi(cmd.args[0].c_str()) * 1000;
        }
        out << ");";
    }
    else if (cmd.args[1] == "ms")
    {
        out << "delay(";
        if (isVar)
        {
            out << cmd.args[0];
        }
        else
        {
            out << atoi(cmd.args[0].c_str());
        }
        out << ");";
    }
    else if (cmd.args[1] == "u")
    {
        out << "delayMicroseconds(";
        if (isVar)
        {
            out << cmd.args[0];
        }
        else
        {
            out << atoi(cmd.args[0].c_str());
        }
        out << ");";
    }
}
