cmake_minimum_required(VERSION 3.10)
project(trie)

file(GLOB SOURCES "*.c")

add_executable(trie ${SOURCES})

target_compile_options(trie PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)