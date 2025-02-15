#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum VarType
{
    INVALID,
    VOID,
    INT,
    BOOL,
    FLOAT,
    BYTE,
    LONG,
    STRING,
    CHAR,
    OBJ
};

struct Var
{
    VarType type;
    std::string name;
    std::string initialValue;
};

struct Cmd
{
    std::string name;
    std::vector<std::string> args;
    bool isVar = false;
    void* subCmd = nullptr;
    bool canDeleteSubCmd = true;

    ~Cmd()
    {
        if (subCmd != nullptr && canDeleteSubCmd) delete (Cmd*)subCmd;
    }
};

struct FuncHeader
{
    std::string returnType;
    std::vector<std::pair<std::string, VarType>> params;
};

struct Func
{
    std::string name;
    FuncHeader header;
    std::vector<Var> vars;
    std::vector<Cmd> cmds;
};

struct Program
{
    std::vector<Var> globalVars;
    std::unordered_map<std::string, Func> funcs;
    std::vector<std::string> callOnSetupFuncs;
    std::vector<std::string> includes;
};