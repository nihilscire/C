cmake_minimum_required(VERSION 3.10)
project(REDBLACK)

file(GLOB SOURCES "*.c")

add_executable(rb ${SOURCES})

target_compile_options(rb PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)