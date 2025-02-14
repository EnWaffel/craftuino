#include "Sketch.h"
#include "Logger.h"
#include "command/Commands.h"
#include "Generator.h"
#include "Config.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

Sketch* Sketch::CUR_SKETCH = nullptr;

Sketch::Sketch()
{
    cmdMgr.AddCommand("delay", new DelayCmd);
    cmdMgr.AddCommand("pinmode", new PinModeCmd);
    cmdMgr.AddCommand("setpin", new SetPinCmd);
    cmdMgr.AddCommand("function", new FunctionCmd);
    cmdMgr.AddCommand("var", new VarCmd);
    cmdMgr.AddCommand("gvar", new GVarCmd);
    cmdMgr.AddCommand("set", new SetCmd);
    cmdMgr.AddCommand("execute", new ExecuteCmd);
    cmdMgr.AddCommand("getpin", new GetPinCmd);
    cmdMgr.AddCommand("serial", new SerialCmd);

    CUR_SKETCH = this;
}

Sketch::~Sketch()
{
    for (const auto& v : streams)
    {
        if (v.second->is_open()) v.second->close();
        delete v.second;
    }
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

    fs::path setupPath = folderPath + "/setup.cino";
    fs::path loopPath = folderPath + "/loop.cino";

    if (!fs::exists(setupPath))
    {
        spdlog::error("Sketch: Couldn't access setup.cino: '" + setupPath.string() + "'");
        return 1;
    }

    if (!fs::exists(loopPath))
    {
        spdlog::error("Sketch: Couldn't access loop.cino: '" + loopPath.string() + "'");
        return 1;
    }

    std::vector<fs::path> functionPaths;
    for (const auto& v : fs::directory_iterator(sketchFolder))
    {
        std::string fileName; 
#ifdef __linux__
        fileName = v.path().filename();
#endif
#ifdef _WIN32
        fileName = v.path().filename().string();
#endif
        //if (fileName.find("setup.cino") != std::string::npos || fileName.find("loop.cino") != std::string::npos) continue;

        if (EndsWith(fileName, ".cino"))
        {
            functionPaths.push_back(v.path().string());
        }
    }

    for (const fs::path& path : functionPaths)
    {
        std::ifstream* in = new std::ifstream(path);
        if (!in->is_open())
        {
            spdlog::error("Sketch: Couldn't open {0}: '{1}'", path.filename().string(), path.string());
            return 1;
        }
        streams.insert({ path.filename().string(), in });
    }

    spdlog::info("Parser: Parsing started");

    int result;

    // Parse

    for (const auto& v : streams)
    {
        int result;
        if ((result = Parse(*v.second)) != 0)
        {
            return result;
        }
    }

    spdlog::info("Parser: Parsing finished with 0 warnings and 0 errors");

    // Reopen for compilation
    for (auto& v : streams)
    {
        v.second->close();
        v.second->open(sketchFolder + "/" + v.first);
    }

    // Compile

    spdlog::info("Compiler: Compiling started");

    for (const auto& v : streams)
    {
        int result;
        if ((result = Compile(*v.second, v.first)) != 0)
        {
            return result;
        }
    }

    for (const auto& v : streams)
    {
        if (v.second->is_open()) v.second->close();
        delete v.second;
    }
    streams.clear();

    for (std::string& f : prog.callOnSetupFuncs)
    {
        if (prog.funcs.count(f) < 1)
        {
            spdlog::error("Compiler: Link error: (function header '-cos') Function '{0}' not found", f);
            return 1;
        }

        Func& func = prog.funcs.at(f);
        if (StrToVarType(func.header.returnType) != VarType::VOID)
        {
            spdlog::error("Compiler: [FUNC:{0}] Link error: (function header '-cos') Functions declared to be called on setup should have a return type of 'void'", f);
            return 1;
        }
        
        if (!func.header.params.empty())
        {
            spdlog::error("Compiler: [FUNC:{0}] Link error: (function header '-cos') Functions declared to be called on setup should not have any parameters", f);
            return 1;
        }
    }

    spdlog::info("Compiler: Compiling finished with 0 warnings and 0 errors");

    Generator gen(sketchFolder, cmdMgr);
    
    if ((result = gen.GenerateCXXCode(prog)) != 0)
    {
        spdlog::error("Sketch: Generation terminated");
        return result;
    }

    if (!onlyGenerate)
    {
        // Build
        spdlog::info("Sketch: Build started");

        if ((result = Build()) != 0)
        {
            spdlog::error("Sketch: Build terminated");
            return result;
        }

        spdlog::info("Sketch: Build finished");

        // Upload
        spdlog::info("Sketch: Upload started");

        if ((result = Upload()) != 0)
        {
            spdlog::error("Sketch: Upload terminated");
            return result;
        }

        spdlog::info("Sketch: Upload finished");
    }
    
    spdlog::info("Sketch: Work done. Bye!");

    return 0;
}

int Sketch::Compile(std::ifstream& in, const std::string& _funcName)
{
    std::string funcName = _funcName;
    replace(funcName, ".cino", "");

    Func func{};
    std::pair<int, FuncHeader> head = ReadFunctionHeader(in, funcName);
    if (head.first != 0)
    {
        return 1;
    }
    func.header = head.second;
    func.name = funcName;

    prog.funcs.insert({ funcName, func });

    int lineNum = 1;
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            lineNum++;
            continue;
        }
        if (line.front() == '#')
        {
            lineNum++;
            continue;
        }
        if (line.front() == '-')
        {
            lineNum++;
            continue;
        }
        
        if (line.at(0) != '/')
        {
            spdlog::error("Compiler: [{0}:{1}] Syntax error: Expected '/' at beginning", funcName, lineNum);
            return 1;
        }
        
        if (line.size() < 2)
        {
            spdlog::error("Compiler: [{0}:{1}] Syntax error: A '/' should always be followed by a command", funcName, lineNum);
            return 1;
        }

        int result;
        if ((result = cmdMgr.CompileLine(prog, line, lineNum, funcName, false)) != 0)
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
        spdlog::error("Sketch: Build failed");
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
        spdlog::error("Sketch: Upload failed");
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
        if (line.front() == '-') continue;

        if (line.at(0) != '/')
        {
            spdlog::error("Parser: [{0}] Syntax error: Expected '/' at beginning", lineNum + 1);
            return 1;
        }

        if (line.size() < 2)
        {
            spdlog::error("Parser: [{0}] Syntax error: A '/' should always be followed by a command", lineNum + 1);
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

std::pair<int, FuncHeader> Sketch::ReadFunctionHeader(std::ifstream& in, const std::string& func)
{
    FuncHeader header{};
    header.returnType = "void";

    int lineNum = 1;
    do
    {
        std::string line;
        std::getline(in, line);

        if (line.empty())
        {
            lineNum++;
            continue;
        }
        if (line.front() == '#')
        {
            lineNum++;
            continue;
        }

        if (line.at(0) != '-')
        {
            spdlog::error("Parser: [{0}:{1}] Syntax error: Expected '-' at function header", func, lineNum);
            return { 1, header };
        }

        if (line.size() < 2)
        {
            spdlog::error("Parser: [{0}:{1}] Syntax error: No function header option", func, lineNum);
            return { 1, header };
        }

        std::vector<std::string> split = SplitString(line);
        if (split.size() < 2)
        {
            spdlog::error("Parser: [{0}:{1}] Syntax error: No function header option", func, lineNum);
            return { 1, header };
        }

        if (split[0] == "-rt")
        {
            header.returnType = split[1];
        }
        else if (split[0] == "-p")
        {
            std::vector<std::string> paramPairs = SplitString(split[1], ";");

            for (const std::string& pair : paramPairs)
            {
                std::vector<std::string> param = SplitString(pair, ":");
                if (param.size() < 2)
                {
                    spdlog::error("Parser: [{0}:{1}] Syntax error: Var type and name expected for function parameter ({2})", func, lineNum, pair);
                    return { 1, header };
                }

                VarType type = StrToVarType(param[0]);
                if (type == VarType::INVALID)
                {
                    spdlog::error("Parser: [{0}:{1}] Syntax error: Invalid Var type: '{3}' ({2})", func, lineNum, pair, param[0]);
                    return { 1, header };
                }

                header.params.push_back({ param[1], type });
            }
        }
        else if (split[0] == "-cos")
        {
            for (int i = 1; i < split.size(); i++)
            {
                std::string s = split[i];
                if (s == ":this:")
                {
                    s = func;
                }
                prog.callOnSetupFuncs.push_back(s);
            }
        }
        else
        {
            spdlog::error("Parser: [{0}:{1}] Syntax error: Unknown function header option: '{2}'", func, lineNum, split[0]);
            return { 1, header };
        }

        lineNum++;
    } while (in.peek() == '-');

    return { 0, header };
}
