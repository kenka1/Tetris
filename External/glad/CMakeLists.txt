cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(glad)

add_library(glad
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c)

target_include_directories(glad
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)


