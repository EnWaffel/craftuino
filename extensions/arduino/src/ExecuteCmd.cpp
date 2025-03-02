#include "ExecuteCmd.h"

int ExecuteCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 1)
    {
        spdlog::error("ExecuteCmd: Usage: /execute <if>");
        return 1;
    }

    if (args[0] == "if" && args.size() < 4)
    {
        spdlog::error("ExecuteCmd: Usage: /execute if [? (literal, var)] ['=', '|', '&', '>', '<', '>=', '=<' (option)] [? (literal, var)]");
        return 1;
    }

    return 0;
}

std::pair<int, Cmd> ExecuteCmd::Compile(Program& prog, Func& func, const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "execute";
    cmd.args = args;

    std::string line;
    for (int i = 5; i < args.size(); i++)
    {
        line += args[i];
        line.push_back(' ');
    }

    cmd.subCommands.push_back(api->CompileLine(line, func.name, -1));

    return { 0, cmd };
}

void ExecuteCmd::Generate(Program& prog, Func& func, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args[0] == "if")
    {
        out << "if(";
        out << cmd.args[1];
        
        if (cmd.args[2] == "=")
        {
            out << "==";
        }
        else if (cmd.args[2] == "|")
        {
            out << "||";
        }
        else if (cmd.args[2] == "&")
        {
            out << "&&";
        }
        else
        {
            out << cmd.args[2];
        }

        out << cmd.args[3];
        out << "){";
        LN;

        api->GenerateCommand(cmd.subCommands.front().get(), func, out);

        LN;
        out << "}";
    }
}
