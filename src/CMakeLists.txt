cmake_minimum_required(VERSION 3.0)

file(GLOB sources "*.cpp")

include_directories(
    "/usr/local/include/"
)

add_library(json-parser STATIC ${sources})
