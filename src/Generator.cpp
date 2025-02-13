#include "Generator.h"
#include "Utils.h"

#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Generator::Generator(const std::string& sketchFolder, CommandManager& cmdMgr)
{
    this->sketchFolder = sketchFolder;
    this->cmdMgr = &cmdMgr;
}

int Generator::GenerateCXXCode(Program& prog)
{
    spdlog::info("Generator: Generation started");
    numToGenerate = prog.funcs.size();

#ifdef __linux__
	sketchName = fs::path(sketchFolder).filename();
#endif
#ifdef _WIN32
	sketchName = fs::path(sketchFolder).filename().string();
#endif
    sketchOutPath = "/build/gen/" + sketchName;

    if (fs::exists(sketchFolder + "/build"))
    {
        fs::remove_all(sketchFolder + "/build");
    }

    fs::create_directories(sketchFolder + "/build/gen/" + sketchName);
    fs::create_directories(sketchFolder + "/build/bin");

    std::string path = sketchFolder + sketchOutPath + "/" + sketchName + ".ino";
    out.open(path);
    if (!out.is_open())
    {
        spdlog::error("Generator: Couldn't write to {0}", path);
        return 1;
    }

    out << "#include <Arduino.h>";
    LN;

    for (auto& v : prog.funcs)
    {
        if (v.first == "setup" || v.first == "loop") continue;
        GenFuncDeclaration(prog, v.second, v.first);
        out << ';';
        LN;
    }

    for (auto& v : prog.funcs)
    {
        int result;
        if ((result = GenFunc(prog, v.second, v.first)) != 0)
        {
            return result;
        }
        LN;
    }

    out.close();
    spdlog::info("Generator: Generation finished");

    return 0;
}

int Generator::GenFunc(Program& prog, Func& func, const std::string& name)
{
    LogGenNext(name);

    GenFuncDeclaration(prog, func, name);
    
    out << '{';
    LN;

    for (Cmd& cmd : func.cmds)
    {
        cmdMgr->GenerateCommand(prog, cmd, out);
        LN;
    }

    out << '}';

    return 0;
}

void Generator::GenFuncDeclaration(Program& prog, Func& func, const std::string& name)
{
    out << func.header.returnType;
    SPACE;
    out << name;
    out << '(';

    int i = 0;
    for (const auto& v : func.header.params)
    {
        out << VarTypeToStr(v.second);
        SPACE;
        out << v.first;

        if (i < func.header.params.size() - 1)
        {
            out << ',';
        }
        i++;
    }

    out << ')';
}

float Generator::GetGenPercent()
{
    return (numGenerated / numToGenerate) * 100.0f;
}

void Generator::LogGenNext(const std::string& path)
{
    numGenerated++;
    spdlog::info("Generator: [ {0}%] Generating: {1}", (int)GetGenPercent(), path);
}
