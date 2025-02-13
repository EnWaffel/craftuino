#pragma once

#include "Program.h"

#include <string>
#include <vector>

#define LN out << '\n'
#define SPACE out << ' '

bool IsNumber(const std::string& s);
int ExecuteProgram(const std::string& name, std::vector<std::string>& args);
bool EndsWith(const std::string& fullString, const std::string& ending);
std::vector<std::string> SplitString(const std::string& str);
std::vector<std::string> SplitString(const std::string& s, const std::string& delimiter);
VarType StrToVarType(const std::string& str);
std::string VarTypeToStr(VarType type);