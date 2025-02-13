#include "command/cmds/PinModeCmd.h"

int PinModeCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("PinModeCmd: Usage: /pinmode [pin(int)] [INPUT, OUTPUT(string)]");
        return 1;
    }

    if (!IsNumber(args[0]))
    {
        spdlog::error("PinModeCmd: ARG[1] should be an integer");
        spdlog::error("PinModeCmd: Usage: /pinmode [pin(int)] [INPUT, OUTPUT(string)]");
        return 1;
    }

    if (args[1] != "INPUT" && args[1] != "OUTPUT")
    {
        spdlog::error("PinModeCmd: ARG[2] should be a string ('INPUT' or 'OUTPUT')");
        spdlog::error("PinModeCmd: Usage: /pinmode [pin(int)] [INPUT, OUTPUT(string)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> PinModeCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "pinmode";
    cmd.args = args;

    return { 0, cmd };
}

void PinModeCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args[1] == "INPUT")
    {
        out << "pinMode(" << atoi(cmd.args[0].c_str()) << ", INPUT);";
    }
    else if (cmd.args[1] == "OUTPUT")
    {
        out << "pinMode(" << atoi(cmd.args[0].c_str()) << ", OUTPUT);";
    }
}
