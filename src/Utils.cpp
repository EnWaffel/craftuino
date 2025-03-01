#include "Utils.h"
#include "Logger.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <regex>

bool IsNumber(const std::string& str)
{
    bool hasDecimal = false;
    bool hasDigit = false;
    
    if (str.empty()) return false;
    
    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    
    for (size_t i = start; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            hasDigit = true;
        } else if (str[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else {
            return false;
        }
    }
    
    return hasDigit;
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
    std::regex rgx(R"((\"[^\"]*\")|\S+)");

    auto words_begin = std::sregex_iterator(str.begin(), str.end(), rgx);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        result.push_back((*i).str());
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
    else if (str == "STRING" || str == "string")
    {
        return VarType::STRING;
    }
    else if (str == "CHAR" || str == "char")
    {
        return VarType::CHAR;
    }
    else if (str == "VOID" || str == "void")
    {
        return VarType::VOID;
    }

    return VarType::INVALID;
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
    else if (type == VarType::STRING)
    {
        return "const char*";
    }
    else if (type == VarType::CHAR)
    {
        return "char";
    }

    return "auto";
}   

VarType EstimateVarTypeByValue(const std::string& value)
{
    if (value.empty()) return VarType::INVALID;

    if (IsNumber(value))
    {
        if (value.find(".") != std::string::npos)
        {
            return VarType::FLOAT;
        }
        else
        {
            return VarType::INT;
        }
    }
    else
    {
        std::string lower = ToLowercase(value);
        if (lower == "true" || lower == "false")
        {
            return VarType::BOOL;
        }

        if (value.size() >= 2)
        {
            if (value.front() == '\"' || value.back() == '\"')
            {
                return VarType::STRING;
            } else if (value.front() == '\'' || value.back() == '\'')
            {
                return VarType::CHAR;
            }
        }
    }

    return VarType::OBJ;
}

std::string ToLowercase(const std::string& str)
{
    std::string result;
    for (char c : str) result.push_back(tolower(c));
    return result;
}

Var* GetVarByName(Program* prog, Func* func, const std::string& name)
{
    for (Var& v : prog->globalVars)
    {
        if (v.name == name) return &v;
    }

    for (Var& v : func->vars)
    {
        if (v.name == name) return &v;
    }

    return nullptr;
}

std::string ReplaceString(std::string str, const std::string& from, const std::string& to)
{
    if (from.empty()) {
        return str;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}
