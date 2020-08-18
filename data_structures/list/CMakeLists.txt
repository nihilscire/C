cmake_minimum_required(VERSION 3.10)
project(LIST)

file(GLOB SOURCES "*.c")

add_executable(list ${SOURCES})

target_compile_options(list PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)