cmake_minimum_required(VERSION 4.0)
project(chip8Emulator)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(chip8Emulator WIN32 MACOSX_BUNDLE main.cpp chip8.cpp chip8.h platform.cpp)

target_link_libraries(chip8Emulator PRIVATE raylib)

target_include_directories(chip8Emulator PRIVATE /opt/homebrew/include /usr/local/include)

file(COPY font.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(APPLE)
    add_custom_command(TARGET chip8Emulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/font.ttf"
            "$<TARGET_FILE_DIR:chip8Emulator>/font.ttf"
    )

    add_custom_command(TARGET chip8Emulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/bin"
            "$<TARGET_FILE_DIR:chip8Emulator>/bin"
    )
endif()