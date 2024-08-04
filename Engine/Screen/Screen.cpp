#include <iostream>
#include <stdlib.h>

#include "glm/gtc/matrix_transform.hpp"

#include "Screen/Screen.h"
#include "Shape/Shape.h"

Screen::Screen():
    window(nullptr), projection(glm::scale(glm::mat4(1.0f), glm::vec3(2.0 / width, 2.0 / height, 0.0)))
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

    glViewport(0, 0, width, height);
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);
}

void Screen::size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Screen::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, 1);
}

void Screen::callMove(Shape* obj)
{
    if(glfwGetKey(window, GLFW_KEY_D) == 1)
        obj->move(0);
    if(glfwGetKey(window, GLFW_KEY_A) == 1)
        obj->move(1);
    if(glfwGetKey(window, GLFW_KEY_S) == 1)
        obj->move(2);
    obj->UpdateTransform();
}