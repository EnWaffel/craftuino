cmake_minimum_required(VERSION 3.5)

add_subdirectory(depend/spdlog)
add_subdirectory(ext-api)
add_subdirectory(extensions/arduino)

set(VER 0.2.0)

add_compile_definitions(-DCRAFTUINO_VERSION="${VER}")

project(craftuino LANGUAGES CXX VERSION ${VER})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(BUILD_SHARED_LIBS OFF)

add_executable(craftuino
"src/Main.cpp"
"src/Config.cpp"
"src/Sketch.cpp"
"src/command/CommandManager.cpp"
"src/Generator.cpp"
"src/extension/Extension.cpp"
"src/extension/ExtensionAPIImpl.cpp"
)

target_include_directories(craftuino PUBLIC "include" "depend/argparse/include" "depend/inifile-cpp/include" "ext-api/include")
target_link_libraries(craftuino PRIVATE spdlog ext-api kernel32)