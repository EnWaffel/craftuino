#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum VarType
{
    VOID,
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

struct FuncHeader
{
    std::string returnType;
    std::vector<std::pair<std::string, VarType>> params;
};

struct Func
{
    FuncHeader header;
    std::vector<Var> vars;
    std::vector<Cmd> cmds;
};

struct Program
{
    std::vector<Var> globalVars;
    std::unordered_map<std::string, Func> funcs;
};