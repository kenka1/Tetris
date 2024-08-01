#include <iostream>
#include <stdlib.h>

#include "Screen/Screen.h"


Screen::Screen():
    window(nullptr)
{
    initialize();
}

Screen::~Screen()
{
    glfwTerminate();
}

void Screen::initialize()
{
    if(!glfwInit())
    {
        std::cerr << "ERROR init glfw" << std::endl;
        std::abort();
    }

    window = glfwCreateWindow(width, height, "Tetris", nullptr, nullptr);

    if(!window)
    {
        std::cerr << "ERROR creation window" << std::endl;
        glfwTerminate();
        std::abort();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ScreenCONST::VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ScreenCONST::VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, ScreenCONST::OPENGL_PROFILE);

    glfwMakeContextCurrent(window);
    
    if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "ERROR load glad" << std::endl;
        glfwTerminate();
        std::abort();
    }
}