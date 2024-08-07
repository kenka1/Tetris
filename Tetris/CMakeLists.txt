cmake_minimum_required(VERSION 3.28)

project(TetrisSource)

add_library(TetrisSource
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Grid/Grid.cpp)

target_include_directories(TetrisSource
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source)

target_include_directories(TetrisSource
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine)

target_include_directories(TetrisSource
    PUBLIC
    ${CMAKE_SOURCE_DIR}/External/include)