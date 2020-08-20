cmake_minimum_required(VERSION 3.10)
project(CONSPROD)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package( Threads REQUIRED )

set(LIB_SOURCES queue.c list.c util.c)
set(LIB_HEADERS queue.h list.h util.h)

add_library(libqueue ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(consprod_sem consprod_sem.c)
add_executable(consprod_condvar consprod_condvar.c)

target_compile_options(consprod_sem PUBLIC -Wall -Wextra -Wpedantic)
target_compile_options(consprod_condvar PUBLIC -Wall -Wextra -Wpedantic)

target_link_libraries(consprod_sem Threads::Threads libqueue)
target_link_libraries(consprod_condvar Threads::Threads libqueue)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE on)