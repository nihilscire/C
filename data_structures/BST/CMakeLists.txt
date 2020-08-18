cmake_minimum_required(VERSION 3.10)
project(BST)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(main ${SOURCES})

target_compile_options(main PUBLIC -Wall -Wextra -Wpedantic)
