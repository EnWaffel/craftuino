#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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

struct CXXFile
{
    std::vector<Var> vars;
    std::vector<Cmd> cmds;
};

struct Program
{
    std::vector<Var> vars;
    std::vector<Cmd> setupCmds;
    std::vector<Cmd> loopCmds;
    std::vector<CXXFile> others;
};