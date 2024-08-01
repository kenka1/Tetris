cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(Engine)

add_library(Engine
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Block/Block.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMode/GameMode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Program/Program.cpp)

target_include_directories(Engine
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine)

target_include_directories(Engine
    PUBLIC
    ${CMAKE_SOURCE_DIR}/External/include)