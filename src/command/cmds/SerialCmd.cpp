#include "command/cmds/SerialCmd.h"

int SerialCmd::CheckSyntax(const std::vector<std::string>& args)
{
    if (args.size() < 1)
    {
        spdlog::error("SerialCmd: Usage: /serial <begin, print, println>");
        return 1;
    }

    if (args[0] == "begin")
    {
        if (args.size() < 2)
        {
            spdlog::error("SerialCmd: Usage: /serial begin [baudRate(int)]"); 
            return 1;
        }
    }
    else if (args[0] == "print")
    {
        if (args.size() < 2)
        {
            spdlog::error("SerialCmd: Usage: /serial print [message(any)]"); 
            return 1;
        }
    }
    else if (args[0] == "println")
    {
        if (args.size() < 2)
        {
            spdlog::error("SerialCmd: Usage: /serial println [message(any)]"); 
            return 1;
        }
    }

    return 0;
}

std::pair<int, Cmd> SerialCmd::Compile(const std::vector<std::string>& args)
{
    Cmd cmd{};
    cmd.name = "serial";
    cmd.args = args;

    return { 0, cmd };
}

void SerialCmd::Generate(Program& prog, Cmd& cmd, std::ofstream& out)
{
    if (cmd.args[0] == "begin")
    {
        out << "Serial.begin(";
        out << cmd.args[1];
        out << ");";
    }
    else if (cmd.args[0] == "print")
    {
        out << "Serial.print(";
        out << cmd.args[1];
        out << ");";
    }
    else if (cmd.args[0] == "println")
    {
        out << "Serial.println(";
        out << cmd.args[1];
        out << ");";
    }
}
