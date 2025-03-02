cmake_minimum_required(VERSION 3.5)

project(ext-arduino LANGUAGES CXX)

add_library(ext-arduino SHARED
"ext.def"
"src/ExtMain.cpp"
"src/DelayCmd.cpp"
"src/PinModeCmd.cpp"
"src/SetPinCmd.cpp"
"src/FunctionCmd.cpp"
"src/VarCmd.cpp"
"src/GVarCmd.cpp"
"src/SetCmd.cpp"
"src/ExecuteCmd.cpp"
"src/GetPinCmd.cpp"
"src/SerialCmd.cpp"
)

target_include_directories(ext-arduino PUBLIC "include" "../../depend/spdlog/include" "../../ext-api/include")
target_link_directories(ext-arduino PRIVATE "../../build/depend/spdlog" "../../build/ext-api")
target_link_libraries(ext-arduino PRIVATE spdlog ext-api)