cmake_minimum_required(VERSION 4.0)
project(chip8Emulator)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)


add_executable(chip8Emulator main.cpp
        chip8.cpp
        chip8.h)

target_link_libraries(chip8Emulator PRIVATE raylib)

target_include_directories(chip8Emulator PRIVATE /opt/homebrew/include /usr/local/include)


