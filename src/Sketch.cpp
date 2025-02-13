#include "Sketch.h"
#include "Logger.h"
#include "command/Commands.h"
#include "Generator.h"
#include "Config.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Sketch::Sketch()
{
    cmdMgr.AddCommand("delay", new DelayCmd);
    cmdMgr.AddCommand("pinmode", new PinModeCmd);
    cmdMgr.AddCommand("setpin", new SetPinCmd);
}

Sketch::~Sketch()
{
}

int Sketch::DoDir(const std::string folderPath)
{
    sketchFolder = folderPath;
#ifdef __linux__
	sketchName = fs::path(sketchFolder).filename();
#endif
#ifdef _WIN32
	sketchName = fs::path(sketchFolder).filename().string();
#endif

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

    spdlog::info("Parser: Parsing started");

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

    setupIn.close();
    loopIn.close();
    setupIn.open(setupPath);
    loopIn.open(loopPath);

    spdlog::info("Parser: Parsing finished");

    // Compile

    spdlog::info("Compiler: Compiling started");

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

    spdlog::info("Compiler: Compiling finished");

    Generator gen(sketchFolder, cmdMgr);
    
    if ((result = gen.GenerateCXXCode(prog)) != 0)
    {
        spdlog::error("Sketch: Generation terminated");
        return result;
    }

    // Build
    spdlog::info("Builder: Build started");

    if ((result = Build()) != 0)
    {
        spdlog::error("Sketch: Build terminated");
        return result;
    }

    spdlog::info("Builder: Build finished");

    // Upload
    spdlog::info("Uploader: Upload started");

    if ((result = Upload()) != 0)
    {
        spdlog::error("Sketch: Upload terminated");
        return result;
    }

    spdlog::info("Uploader: Upload finished");

    spdlog::info("Sketch: Finished! Bye!");

    return 0;
}

int Sketch::Compile(std::ifstream& in, bool isSetup)
{
    int lineNum = 1;
    std::string line;
    while (std::getline(in, line))
    {
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
    }

    return 0;
}

int Sketch::Build()
{
    std::string buildPath = sketchFolder + "/build/bin";
    std::string path = sketchFolder + "/build/gen/" + sketchName;
    std::vector<std::string> args;

    args.push_back("compile");
    args.push_back("--fqbn");
    args.push_back(Config::board);
    args.push_back("--build-path");
    args.push_back(buildPath);
    args.push_back(path);
    args.push_back(Config::verbose ? "--verbose" : "");

    int code = ExecuteProgram(Config::arduinoCompilerPath, args);
    
    if (code != 0)
    {
        spdlog::error("Builder: Build failed");
        return code;
    }

    return 0;
}

int Sketch::Upload()
{
    std::string hexPath = sketchFolder + "/build/bin/" + sketchName + ".ino.hex";
    std::vector<std::string> args;

    args.push_back("\"-C" + Config::avrdudeConfigPath + "\"");
    args.push_back("-V");
    args.push_back("-p" + Config::chip);
    args.push_back("-c" + Config::programmer);
    args.push_back("-P" + Config::port);
    args.push_back("-b" + Config::baudRate);
    args.push_back(Config::verbose ? "-v" : "");
    args.push_back("\"-Uflash:w:" + hexPath + "\"");

    int code = ExecuteProgram(Config::avrdudePath, args);
    
    if (code != 0)
    {
        spdlog::error("Uploader: Upload failed");
        return code;
    }

    return 0;
}

int Sketch::Parse(std::ifstream& in)
{
    int lineNum = 1;
    std::string line;
    while (std::getline(in, line))
    {
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
    }

    return 0;
}
