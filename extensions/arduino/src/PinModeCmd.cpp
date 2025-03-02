#include "PinModeCmd.h"

int PinModeCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("PinModeCmd: Usage: /pinmode [pin(int, var)] [INPUT, OUTPUT(option)]");
        return 1;
    }

    if (args[1] != "INPUT" && args[1] != "OUTPUT")
    {
        spdlog::error("PinModeCmd: ARG[2] should be an option ('INPUT' or 'OUTPUT')");
        spdlog::error("PinModeCmd: Usage: /pinmode [pin(int, var)] [INPUT, OUTPUT(option)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> PinModeCmd::Compile(Program& prog, Func& func, const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "pinmode";
    cmd.args = args;

    return { 0, cmd };
}

void PinModeCmd::Generate(Program& prog, Func& func, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args[1] == "INPUT")
    {
        out << "pinMode(" << cmd.args[0] << ", INPUT);";
    }
    else if (cmd.args[1] == "OUTPUT")
    {
        out << "pinMode(" << cmd.args[0] << ", OUTPUT);";
    }
}
