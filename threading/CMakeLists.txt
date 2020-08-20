cmake_minimum_required(VERSION 3.10)
project(CONSPROD)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package( Threads REQUIRED )

file(GLOB SOURCES "*.c")

add_executable(consprod ${SOURCES})

target_compile_options(consprod PUBLIC -Wall -Wextra -Wpedantic)

target_link_libraries(consprod Threads::Threads)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)