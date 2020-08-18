cmake_minimum_required(VERSION 3.10)
project(graph)

file(GLOB SOURCES "*.c")

add_executable(graph ${SOURCES})

target_compile_options(graph PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)