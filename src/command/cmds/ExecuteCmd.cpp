#include "command/cmds/ExecuteCmd.h"
#include "Sketch.h"

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

std::pair<int, Cmd> ExecuteCmd::Compile(const std::vector<std::string>& args)
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

    cmd.subCmd = new Cmd;
    cmd.canDeleteSubCmd = false;

    Sketch::CUR_SKETCH->cmdMgr.CompileLine(*prog, line, -1, func->name, true, (Cmd*)cmd.subCmd);

    return { 0, cmd };
}

void ExecuteCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
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

        Sketch::CUR_SKETCH->cmdMgr.GenerateCommand(prog, *((Cmd*)cmd.subCmd), out);

        LN;
        out << "}";
    }
}
