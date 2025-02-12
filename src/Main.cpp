#include "Config.h"
#include "Sketch.h"

#include <argparse/argparse.hpp>

// Compile Command:
/*
arduino-cli compile --fqbn arduino:avr:uno --build-path %sketchFolder%/build/bin %sketchFolder%/build/gen --verbose
*/

// Upload Command:
/*
avrdude "-C/home/%USER%/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf" -v -V -patmega328p -carduino "-P/dev/ttyACM0" -b115200 -D "-Uflash:w:/home/enwaffel/.cache/arduino/sketches/AA8C6A60D40431D08EEC43E783B5FDE2/Blink.ino.hex:i"
*/

int main(int argc, char *argv[])
{
    argparse::ArgumentParser args("craftuino", CRAFTUINO_VERSION);

    args.add_argument("-c").help("Path to the config file").default_value("config.ini").nargs(1);
    args.add_argument("-s").help("Path to the sketch folder").required().nargs(1);
  
    try {
      args.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << args;
      return 1;
    }
  
    int result;
    if ((result = Config::Load(args.get("-c"))) != 0)
    {
        return result;
    }

    Sketch sketch;
    if ((result = sketch.DoDir(args.get("-s"))) != 0)
    {
        return result;
    }
  
    return 0; 
}
