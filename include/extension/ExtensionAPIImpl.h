#pragma once

#include "ExtensionAPI.h"

class ExtensionAPIImpl : public ExtensionAPI
{
public:
    void RegisterCommand(const std::string& name, Command* cmd) override;
    std::shared_ptr<Cmd> CompileLine(const std::string& line, const std::string& func, int lineNum) override;
    void GenerateCommand(Cmd* cmd, Func& func, std::ofstream& out) override;
};