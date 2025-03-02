#include "command/CommandManager.h"
#include "Logger.h"

#include <iostream>
#include <vector>

bool VarExists(Program& prog, Func& func, bool global, const std::string& name)
{
    if (global)
    {
        for (Var& var : prog.globalVars)
        {
            if (var.name == name) return true;
        }
    }
    else
    {
        for (Var& var : func.vars)
        {
            if (var.name == name) return true;
        }
    }

    return false;
}

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

int CommandManager::CompileLine(Program& prog, const std::string& line, int lineNum, const std::string& func, bool isSubCmd, Cmd* subCmd)
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
    if ((result = cmd->Compile(prog, prog.funcs.at(func), realSplit)).first != 0)
    {
        spdlog::error("Compiler: [{0}:1] Compilation terminated: Compile error", lineNum, cmdName);
        return result.first;
    }

    if (!isSubCmd)
    {
        prog.funcs.at(func).cmds.push_back(result.second);
    }
    else
    {
        *subCmd = result.second;
    }
    
    if (result.second.isVar)
    {
        Var var = { EstimateVarTypeByValue(result.second.args[1]), result.second.args[0], result.second.args[2] };

        if (VarExists(prog, prog.funcs.at(func), cmdName == "gvar", var.name))
        {
            spdlog::error("Compiler: [{0}:1] Compilation terminated: Compile error: Variable already defined: '{1}'", lineNum + 1, var.name);
            return 1;
        }
        
        if (cmdName == "gvar")
        {
            prog.globalVars.push_back(var);
        }
        else
        {
            prog.funcs.at(func).vars.push_back(var);
        }
    }

    return 0;
}

void CommandManager::GenerateCommand(Program& prog, Func& func, Cmd& cmd, std::ofstream& out)
{
    if (commands.count(cmd.name) < 1)
    {
        out << "//invalid cmd '" << cmd.name << "'";
        return;
    }

    commands.at(cmd.name)->Generate(prog, func, cmd, out);
}
