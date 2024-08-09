cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(Engine)

add_library(Engine
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Shape/Shape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMode/GameMode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Program/Program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameState/GameState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PlayerState/PlayerState.cpp)

target_include_directories(Engine
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/External/include
    ${CMAKE_SOURCE_DIR}/Tetris/Source)