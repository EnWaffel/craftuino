#include "Sketch.h"
#include "Logger.h"
#include "command/Commands.h"
#include "Generator.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Sketch::Sketch()
{
    cmdMgr.AddCommand("delay", new DelayCmd);
}

Sketch::~Sketch()
{
}

int Sketch::DoDir(const std::string folderPath)
{
    sketchFolder = folderPath;

    fs::path setupPath = folderPath + "/setup.ino";
    fs::path loopPath = folderPath + "/loop.ino";

    if (!fs::exists(setupPath))
    {
        spdlog::error("Sketch: Couldn't access setup.ino: '" + setupPath.string() + "'");
        return 1;
    }

    if (!fs::exists(loopPath))
    {
        spdlog::error("Sketch: Couldn't access loop.ino: '" + loopPath.string() + "'");
        return 1;
    }

    std::ifstream setupIn(setupPath);
    if (!setupIn.is_open())
    {
        spdlog::error("Sketch: Couldn't open setup.ino: '" + setupPath.string() + "'");
        return 1;
    }

    std::ifstream loopIn(loopPath);
    if (!setupIn.is_open())
    {
        setupIn.close();
        spdlog::error("Sketch: Couldn't open loop.ino: '" + loopPath.string() + "'");
        return 1;
    }

    // Parse

    int result;
    if ((result = Parse(setupIn)) != 0)
    {
        setupIn.close();
        loopIn.close();
        return result;
    }

    if ((result = Parse(loopIn)) != 0)
    {
        setupIn.close();
        loopIn.close();
        return result;
    }

    // Compile

    if ((result = Compile(setupIn, true)) != 0)
    {
        setupIn.close();
        loopIn.close();
        return result;
    }

    if ((result = Compile(loopIn, false)) != 0)
    {
        setupIn.close();
        loopIn.close();
        return result;
    }

    setupIn.close();
    loopIn.close();

    return 0;
}

int Sketch::Compile(std::ifstream& in, bool isSetup)
{
    int lineNum = 1;
    do
    {
        std::string line;
        std::getline(in, line);

        if (line.empty()) continue;
        if (line.front() == '#') continue;

        if (line.at(0) != '/')
        {
            spdlog::error("Compiler: [{0}:0] Syntax error: Expected '/' at beginning", lineNum);
            return 1;
        }

        if (line.size() < 2)
        {
            spdlog::error("Compiler: [{0}:1] Syntax error: A '/' should always be followed by a command", lineNum);
            return 1;
        }

        int result;
        if ((result = cmdMgr.CompileLine(prog, line, lineNum, isSetup)) != 0)
        {
            return 1;
        }
        
        lineNum++;
    } while (!in.eof());

    Generator gen(sketchFolder, cmdMgr);
    gen.GenerateCXXCode(prog);

    return 0;
}

int Sketch::Upload()
{
}

int Sketch::Parse(std::ifstream& in)
{
    int lineNum = 1;
    do
    {
        std::string line;
        std::getline(in, line);

        if (line.empty()) continue;
        if (line.front() == '#') continue;

        if (line.at(0) != '/')
        {
            spdlog::error("Parser: [{0}:0] Syntax error: Expected '/' at beginning", lineNum);
            return 1;
        }

        if (line.size() < 2)
        {
            spdlog::error("Parser: [{0}:1] Syntax error: A '/' should always be followed by a command", lineNum);
            return 1;
        }

        int result;
        if ((result = cmdMgr.CheckLine(line, lineNum)) != 0)
        {
            return 1;
        }
        
        lineNum++;
    } while (!in.eof());

    return 0;
}
