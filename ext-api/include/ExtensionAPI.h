#pragma once

#include "Program.h"

#include <string>
#include <memory>

#define EXT_MAIN void extension_main(ExtensionAPI* api)

class Command;

class ExtensionAPI
{
public:
    virtual void RegisterCommand(const std::string& name, Command* cmd) = 0;
    virtual std::shared_ptr<Cmd> CompileLine(const std::string& line, const std::string& func, int lineNum) = 0;
    virtual void GenerateCommand(Cmd* cmd, Func& func, std::ofstream& out) = 0;
};

typedef void (*ExtensionMain)(ExtensionAPI* api);