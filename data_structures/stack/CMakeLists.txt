cmake_minimum_required(VERSION 3.10)
project(STACK)

file(GLOB SOURCES "*.c")

add_executable(stack ${SOURCES})

target_compile_options(stack PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)