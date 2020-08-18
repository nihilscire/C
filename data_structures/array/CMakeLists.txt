cmake_minimum_required(VERSION 3.10)
project(ARRAY)

file(GLOB SOURCES "*.c")

add_executable(array ${SOURCES})

target_compile_options(array PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)