#include "Config.h"
#include "Sketch.h"

#include <argparse/argparse.hpp>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser args("craftuino", CRAFTUINO_VERSION);

    args.add_argument("-c").help("Path to the config file").default_value("config.ini.<os>").nargs(1);
    args.add_argument("-s").help("Path to the sketch folder").required().nargs(1);
    args.add_argument("-v").help("Show verbose output").default_value(false).nargs(0);
    args.add_argument("-g").help("Only generate C++ code").default_value(false).nargs(0);
  
    try {
      args.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << args;
      return 1;
    }
  
    Config::verbose = args.is_used("-v");

    std::string configPath = args.get("-c");
    if (configPath == "config.ini.<os>")
    {
#ifdef _WIN32
    configPath = "config.ini.win";
#endif
#ifdef __linux__
    configPath = "config.ini.linux";
#endif
    }

    int result;
    if ((result = Config::Load(configPath)) != 0)
    {
        return result;
    }

    Sketch sketch;
    sketch.onlyGenerate = args.is_used("-g");
    if ((result = sketch.DoDir(args.get("-s"))) != 0)
    {
        return result;
    }
  
    return 0; 
}
