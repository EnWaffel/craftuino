#include "command/cmds/SetPinCmd.h"

int SetPinCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        spdlog::error("SetPinCmd: Usage: /setpin [pin(int)] [HIGH, LOW(string)]");
        return 1;
    }

    if (!IsNumber(args[0]))
    {
        spdlog::error("SetPinCmd: ARG[1] should be an integer");
        spdlog::error("SetPinCmd: Usage: /setpin [pin(int)] [HIGH, LOW(string)]");
        return 1;
    }

    if (args[1] != "HIGH" && args[1] != "LOW")
    {
        spdlog::error("SetPinCmd: ARG[2] should be a string ('HIGH' or 'LOW')");
        spdlog::error("SetPinCmd: Usage: /setpin [pin(int)] [HIGH, LOW(string)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> SetPinCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "setpin";
    cmd.args = args;

    return { 0, cmd };
}

void SetPinCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args[1] == "HIGH")
    {
        out << "digitalWrite(" << atoi(cmd.args[0].c_str()) << ", HIGH);";
    }
    else if (cmd.args[1] == "LOW")
    {
        out << "digitalWrite(" << atoi(cmd.args[0].c_str()) << ", LOW);";
    }
}
