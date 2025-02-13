#include "command/cmds/DelayCmd.h"

int DelayCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u, s(string)]");
        return 1;
    }

    if (!IsNumber(args[0]))
    {
        spdlog::error("DelayCmd: ARG[1] should be an integer");
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u, s(string)]");
        return 1;
    }

    if (args[1] != "ms" && args[1] != "u" && args[1] != "s" )
    {
        spdlog::error("DelayCmd: ARG[2] should be a string ('ms' or 'u' or 's')");
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u, s(string)]");
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
    if (cmd.args[1] == "s")
    {
        out << "delay(" << (atoi(cmd.args[0].c_str()) * 1000) << ");";
    }
    else if (cmd.args[1] == "ms")
    {
        out << "delay(" << atoi(cmd.args[0].c_str()) << ");";
    }
    else if (cmd.args[1] == "u")
    {
        out << "delayMicroseconds(" << atoi(cmd.args[0].c_str()) << ");";
    }
}
