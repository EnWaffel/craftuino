#pragma once

#include "Command.h"

#include <memory>
#include <string>
#include <unordered_map>

class Extension
{
private:
    void* objHandle;
public:
    Extension(void* handle);
    ~Extension();

private:
    static std::unordered_map<std::string, std::unique_ptr<Extension>> loadedExtensions;
public:
    static std::unique_ptr<Extension> LoadObj(const std::string& path);
    static bool LoadAll();
};