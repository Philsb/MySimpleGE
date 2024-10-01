cmake_minimum_required(VERSION 3.1)
project(nlohmann_json)

add_library(nlohmann_json INTERFACE)

target_include_directories(nlohmann_json INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/include")
