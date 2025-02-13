#include "Utils.h"
#include "Logger.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <regex>

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
#ifdef __linux__
	if (WIFEXITED(exitCode)) {
        exitCode = WEXITSTATUS(exitCode);
    }
#endif

    return exitCode;
}

bool EndsWith(const std::string& fullString, const std::string& ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::vector<std::string> SplitString(const std::string& str) {
    std::vector<std::string> result;
    std::regex rgx(R"((\"[^\"]*\")|\S+|\s*)");
    
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), rgx);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string match = (*i).str();
        if (!match.empty() && match.find(' ') == std::string::npos) {
            result.push_back(match);
        }
    }

    return result;
}

std::vector<std::string> SplitString(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

VarType StrToVarType(const std::string& str)
{
    if (str == "INT" || str == "int")
    {
        return VarType::INT;
    }
    else if (str == "BOOL" || str == "bool")
    {
        return VarType::BOOL;
    }
    else if (str == "FLOAT" || str == "float")
    {
        return VarType::FLOAT;
    }
    else if (str == "BYTE" || str == "byte")
    {
        return VarType::BYTE;
    }
    else if (str == "LONG" || str == "long")
    {
        return VarType::LONG;
    }

    return VarType::VOID;
}

std::string VarTypeToStr(VarType type)
{
    if (type == VarType::INT)
    {
        return "int";
    }
    else if (type == VarType::BOOL)
    {
        return "bool";
    }
    else if (type == VarType::FLOAT)
    {
        return "float";
    }
    else if (type == VarType::BYTE)
    {
        return "byte";
    }
    else if (type == VarType::LONG)
    {
        return "long";
    }

    return "void";
}   
