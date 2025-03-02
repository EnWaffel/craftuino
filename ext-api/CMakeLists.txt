cmake_minimum_required(VERSION 3.5)

project(ext-api LANGUAGES CXX)

add_library(ext-api SHARED
"src/Utils.cpp"
)

target_include_directories(ext-api PUBLIC "include" "../depend/spdlog/include")
target_link_directories(ext-api PRIVATE "../build/depend/spdlog")
target_link_libraries(ext-api PRIVATE spdlog)