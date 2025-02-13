#include "Utils.h"
#include "Logger.h"

#include <cstdio>

bool IsNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int ExecuteProgram(const std::string& name, std::vector<std::string>& args)
{
    std::string _args;
    for (const std::string& s : args)
    {
        _args += s + " ";
    }

    std::string command = name + " " + _args;

    FILE* proc = popen(command.c_str(), "r");
    if (!proc) {
        return 1;
    }

    char buf[256];
    while (fgets(buf, sizeof(buf), proc) != nullptr) {
        if (buf[strlen(buf) - 1 == '\n'])
        {
            buf[strlen(buf) - 1] = 0;
        }
        spdlog::info("        {0}", buf);
    }

    int exitCode = pclose(proc);
    if (WIFEXITED(exitCode)) {
        exitCode = WEXITSTATUS(exitCode);
    }

    return exitCode;
}
