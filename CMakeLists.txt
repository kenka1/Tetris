cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(Tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#-fsanitize=address -fsanitize=undefined 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(a 
        ${CMAKE_SOURCE_DIR}/Tetris/src/main.cpp)

target_include_directories(a
        PRIVATE
        ${CMAKE_SOURCE_DIR}/External/include)

target_include_directories(a
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine)

# include(FetchContent)

# FetchContent_Declare(
# 	glm
# 	GIT_REPOSITORY	https://github.com/g-truc/glm.git
# 	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
# )

# FetchContent_MakeAvailable(glm)

# target_link_libraries(a 
#         PRIVATE 
#         glm::glm)

add_subdirectory(${CMAKE_SOURCE_DIR}/External/glfw-3.4)
add_subdirectory(${CMAKE_SOURCE_DIR}/External/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(a
        PRIVATE
        "Engine")

target_link_libraries(a
        PRIVATE
        "glfw"
        "glad"
        "stdc++fs")