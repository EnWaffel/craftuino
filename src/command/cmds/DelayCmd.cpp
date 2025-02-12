#include "command/cmds/DelayCmd.h"

int DelayCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u(string)]");
        return 1;
    }

    if (!IsNumber(args[0]))
    {
        spdlog::error("DelayCmd: ARG[1] should be an integer");
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u(string)]");
        return 1;
    }

    if (args[1] != "ms" && args[1] != "u")
    {
        spdlog::error("DelayCmd: ARG[2] should be a string ('ms' or 'u')");
        spdlog::error("DelayCmd: Usage: /delay [time(int)] [ms, u(string)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> DelayCmd::Compile(const std::vector<std::string>& args)
{
    return { 0, {} };
}

std::string DelayCmd::Generate(Cmd& cmd)
{

}
