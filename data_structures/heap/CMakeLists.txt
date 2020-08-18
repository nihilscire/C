cmake_minimum_required(VERSION 3.10)
project(HEAP)

file(GLOB SOURCES "*.c")

add_executable(heap ${SOURCES})

target_compile_options(heap PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)