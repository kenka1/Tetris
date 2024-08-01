#include <cstdint>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#pragma once

struct ScreenCONST
{
    static const int VERSION_MAJOR = 4;
    static const int VERSION_MINOR = 6;
    static const int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;
    static const int16_t WIDTH = 500;
    static const int16_t HEIGHT = 800;
};

class Screen
{
public:
    Screen();
    ~Screen();

    GLFWwindow* GetWindow() const {return window;}
private:
    int16_t width = ScreenCONST::WIDTH;
    int16_t height = ScreenCONST::HEIGHT;
    GLFWwindow* window;
    void initialize();
};