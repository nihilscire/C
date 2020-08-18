cmake_minimum_required(VERSION 3.10)
project(AVL)

file(GLOB SOURCES "*.c")

add_executable(avl ${SOURCES})

target_compile_options(avl PUBLIC -Wall -Wextra -Wpedantic)