#include "Generator.h"
#include "Utils.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Generator::Generator(const std::string& sketchFolder, CommandManager& cmdMgr)
{
    this->sketchFolder = sketchFolder;
    this->cmdMgr = &cmdMgr;
}

int Generator::GenerateCXXCode(Program& prog)
{
    spdlog::info("Generator: [  0%] Generation started");
    numToGenerate = prog.others.size() + 2;

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

    int result;
    if ((result = GenIno(prog)) != 0)
    {
        return result;
    }

    spdlog::info("Generator: [100%] Generation finished");

    return 0;
}

int Generator::GenIno(Program& prog)
{
    std::string path = sketchFolder + sketchOutPath + "/" + sketchName + ".ino";
    LogGenNext(path);

    std::ofstream out(path);
    if (!out.is_open())
    {
        spdlog::error("Generator: Couldn't write to {0}", path);
        return 1;
    }

    out << "#include <Arduino.h>";
    LN;

    out << "void setup() {";
    LN;

    for (Cmd& cmd : prog.setupCmds)
    {
        cmdMgr->GenerateCommand(prog, cmd, out);
        LN;
    }

    out << "}";

    LN;

    out << "void loop() {";
    LN;

    for (Cmd& cmd : prog.loopCmds)
    {
        cmdMgr->GenerateCommand(prog, cmd, out);
        LN;
    }

    out << "}";

    out.close();
    return 0;void LogGenNext();
}

int Generator::GenFile(Program& prog, const std::string& name)
{
    
}

float Generator::GetGenPercent()
{
    return (numGenerated / numToGenerate) * 100.0f;
}

void Generator::LogGenNext(const std::string& path)
{
    numGenerated++;
    spdlog::info("Generator: [ {0}%] Generating: {1}", GetGenPercent(), path);
}
