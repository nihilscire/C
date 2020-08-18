cmake_minimum_required(VERSION 3.10)
project(QUEUE)

file(GLOB SOURCES "*.c")

add_executable(queue ${SOURCES})

target_compile_options(queue PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)