#include "Program.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "extension/Extension.h"
#include "Logger.h"
#include "Sketch.h"
#include "Utils.h"

#include <ExtensionAPI.h>
#include <filesystem>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::unique_ptr<Extension>> Extension::loadedExtensions;

Extension::Extension(void *handle) : objHandle(handle)
{
}

Extension::~Extension()
{
    FreeLibrary((HMODULE)objHandle);
}

std::unique_ptr<Extension> Extension::LoadObj(const std::string &path)
{
    HMODULE handle = LoadLibraryA(path.c_str());
    if (!handle)
    {
        spdlog::error("Extension: Couldn't load / find extension: {0}", path);
        return nullptr;
    }

    FARPROC addr = GetProcAddress(handle, "extension_main");
    if (!addr)
    {
        spdlog::error("Extension: Couldn't find entry point for extension: {0}", path);
        return nullptr;
    }

    ExtensionMain func = (ExtensionMain)addr;
    func(&Sketch::API);

    return std::move(std::make_unique<Extension>((void*)handle));
}

bool Extension::LoadAll()
{
    for (auto& entry : fs::directory_iterator("extensions"))
    {
        if (entry.is_directory()) continue;
        std::string fileExtension = entry.path().filename().extension().string();

        if (fileExtension == ".dll" || fileExtension == ".so")
        {
            std::unique_ptr<Extension> extension = LoadObj(entry.path().string());
            if (!extension)
            {
                spdlog::error("Extension: Failed to load extension: {0}", entry.path().string());
                return false;
            }

            loadedExtensions.insert({ entry.path().filename().string(), std::move(extension) });

            spdlog::info("Extension: Loaded extension: {0}", entry.path().filename().string());
        }
    }

    return true;
}
