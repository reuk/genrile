cmake_minimum_required(VERSION 3.0)

file(GLOB sources "*.cpp")

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/meta
    "/usr/local/include/"
)

add_executable(tests ${sources})
target_link_libraries(tests genrile)

add_test(genrile tests)
