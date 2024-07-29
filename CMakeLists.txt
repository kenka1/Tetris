cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(Tetris)

add_executable(a 
        ${CMAKE_SOURCE_DIR}/Tetris/src/main.cpp)

target_include_directories(a
        PRIVATE
        ${CMAKE_SOURCE_DIR}/External/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/External/glfw-3.4)
add_subdirectory(${CMAKE_SOURCE_DIR}/External/glad)

target_link_libraries(a
        PRIVATE
        glfw)

target_link_libraries(a
        PRIVATE
        glad)