cmake_minimum_required(VERSION 3.10)
project(SERVER)

set(UTIL "util.c")

add_executable(server "server.c" ${UTIL})
add_executable(client "client.c" ${UTIL})

target_compile_options(server PUBLIC -Wall -Wextra -Wpedantic)
target_compile_options(client PUBLIC -Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)