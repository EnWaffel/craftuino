#pragma once

#include "Program.h"
#include "Logger.h"
#include "Utils.h"

#include <vector>
#include <string>

class Command
{
public:
    Command() = default;
    virtual ~Command() = default;

    virtual int CheckSyntax(const std::vector<std::string>& args) = 0;
    virtual std::pair<int, Cmd> Compile(const std::vector<std::string>& args) = 0;
    virtual std::string Generate(Cmd& cmd) = 0;
};