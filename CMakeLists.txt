cmake_minimum_required(VERSION 3.19)
project(FEM)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRCS src/*.c src/*.cpp src/*.h)

add_executable(FEM ${SRCS})

add_definitions(-DUSE_FILESYSTEM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wno-unused-parameter")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_EXE_LINKER_FLAGS " -static")

target_link_libraries(FEM -static-libgcc -static-libstdc++)