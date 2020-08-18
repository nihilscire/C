cmake_minimum_required(VERSION 3.10)
project(HASH)

file(GLOB SOURCES "*.c")

add_executable(hash ${SOURCES})

target_compile_options(hash PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)