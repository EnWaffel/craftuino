#pragma once

#include "Program.h"
#include "Utils.h"
#include "ExtensionAPI.h"

#include <vector>
#include <string>
#include <fstream>
#include <spdlog/spdlog.h>

class Command
{
public:
    ExtensionAPI* api;
public:
    Command() = default;
    virtual ~Command() = default;

    virtual int CheckSyntax(const std::vector<std::string>& args) = 0;
    virtual std::pair<int, Cmd> Compile(Program& prog, Func& func, const std::vector<std::string>& args) = 0;
    virtual void Generate(Program& prog, Func& func, Cmd& cmd, std::ofstream& out) = 0;
};