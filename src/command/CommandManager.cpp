#include "command/CommandManager.h"
#include "Logger.h"

#include <iostream>
#include <vector>

CommandManager::~CommandManager()
{
    for (const auto& v : commands)
    {
        delete v.second;
    }
}

void CommandManager::AddCommand(const std::string& name, Command* processer)
{
    commands.insert({ name, processer });
}

int CommandManager::CheckLine(const std::string& line, int lineNum)
{
    std::string realLine = line.substr(1);
    std::vector<std::string> split = SplitString(realLine);
    std::vector<std::string> realSplit = std::vector<std::string>(split.begin() + 1, split.end());

    if (split.empty())
    {
        spdlog::error("Parser: [{0}:1] Syntax error: A '/' should always be followed by a command", lineNum);
        return 1;
    }

    std::string cmdName = split.front();

    if (commands.count(cmdName) < 1)
    {
        spdlog::error("Parser: [{0}:1] Unknown/Invalid command: '/{1}'", lineNum, cmdName);
        return 1;
    }

    Command* cmd = commands.at(cmdName);

    int result;
    if ((result = cmd->CheckSyntax(realSplit)) != 0)
    {
        spdlog::error("Parser: [{0}:1] Compilation terminated: Syntax error", lineNum, cmdName);
        return result;
    }

    return 0;
}

int CommandManager::CompileLine(Program& prog, const std::string& line, int lineNum, const std::string& func)
{
    std::string realLine = line.substr(1);
    std::vector<std::string> split = SplitString(realLine);
    std::vector<std::string> realSplit = std::vector<std::string>(split.begin() + 1, split.end());

    if (split.empty())
    {
        spdlog::error("Compiler: [{0}:1] Syntax error: A '/' should always be followed by a command", lineNum);
        return 1;
    }

    std::string cmdName = split.front();

    if (commands.count(cmdName) < 1)
    {
        spdlog::error("Compiler: [{0}:1] Unknown/Invalid command: '/{1}'", lineNum, cmdName);
        return 1;
    }

    Command* cmd = commands.at(cmdName);

    std::pair<int, Cmd> result;
    if ((result = cmd->Compile(realSplit)).first != 0)
    {
        spdlog::error("Compiler: [{0}:1] Compilation terminated: Compile error", lineNum, cmdName);
        return result.first;
    }

    prog.funcs.at(func).cmds.push_back(result.second);

    return 0;
}

void CommandManager::GenerateCommand(Program& prog, Cmd& cmd, std::ofstream& out)
{
    if (commands.count(cmd.name) < 1)
    {
        out << "//invalid cmd '" << cmd.name << "'";
        return;
    }

    commands.at(cmd.name)->Generate(prog, cmd, out);
}
