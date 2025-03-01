cmake_minimum_required(VERSION 3.5)

add_subdirectory(depend/spdlog)

set(VER 0.2.0)

add_compile_definitions(-DCRAFTUINO_VERSION="${VER}")

project(craftuino LANGUAGES CXX VERSION ${VER})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

# Setzt das Standardverhalten für statische Bibliotheken
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# Verhindert die Nutzung dynamischer Bibliotheken
set(BUILD_SHARED_LIBS OFF)

add_executable(craftuino
"src/Main.cpp"
"src/Config.cpp"
"src/Sketch.cpp"
"src/command/CommandManager.cpp"
"src/command/cmds/DelayCmd.cpp"
"src/command/cmds/PinModeCmd.cpp"
"src/command/cmds/SetPinCmd.cpp"
"src/command/cmds/FunctionCmd.cpp"
"src/command/cmds/VarCmd.cpp"
"src/command/cmds/GVarCmd.cpp"
"src/command/cmds/SetCmd.cpp"
"src/command/cmds/ExecuteCmd.cpp"
"src/command/cmds/GetPinCmd.cpp"
"src/command/cmds/SerialCmd.cpp"
"src/Utils.cpp"
"src/Generator.cpp"
"src/command/Command.cpp"
)

target_include_directories(craftuino PUBLIC "include" "depend/argparse/include" "depend/inifile-cpp/include")
target_link_libraries(craftuino PRIVATE spdlog)