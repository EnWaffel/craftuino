cmake_minimum_required(VERSION 3.5)

add_subdirectory(depend/spdlog)

set(VER 0.0.1)

add_compile_definitions(-DCRAFTUINO_VERSION="${VER}")

project(craftuino LANGUAGES CXX VERSION ${VER})

add_executable(craftuino
"src/Main.cpp"
"src/Config.cpp"
"src/Sketch.cpp"
"src/command/CommandManager.cpp"
"src/command/cmds/DelayCmd.cpp"
"src/command/cmds/PinModeCmd.cpp"
"src/command/cmds/SetPinCmd.cpp"
"src/Utils.cpp"
"src/Generator.cpp"
)

target_include_directories(craftuino PUBLIC "include" "depend/argparse/include" "depend/inifile-cpp/include")
target_link_libraries(craftuino PRIVATE spdlog)