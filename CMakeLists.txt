cmake_minimum_required(VERSION 3.7)

project(CHIP8Emulator)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(CHIP8Emulator
    src/main.cpp
    src/Logger.cpp
    src/Application.cpp
)
target_link_libraries(CHIP8Emulator ${SDL2_LIBRARIES})