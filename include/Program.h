#pragma once

#include <string>
#include <vector>

enum VarType
{
    INT,
    BOOL,
    FLOAT,
    BYTE,
    LONG
};

struct Var
{
    VarType type;
    std::string name;
};

struct Cmd
{
    std::string name;
    std::vector<std::string> args;
};

struct Program
{
    std::vector<Var> vars;
    std::vector<Cmd> setupCmds;
    std::vector<Cmd> loopCmds;
};