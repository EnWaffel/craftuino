#include "Generator.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Generator::Generator(const std::string& sketchFolder, CommandManager& cmdMgr)
{
    this->sketchFolder = sketchFolder;
    this->cmdMgr = &cmdMgr;
}

void Generator::GenerateCXXCode(Program& prog)
{
    if (fs::exists(sketchFolder + "/build"))
    {
        fs::remove_all(sketchFolder + "/build");
    }

    fs::create_directories(sketchFolder + "/build/gen");
    fs::create_directories(sketchFolder + "/build/bin");

}
