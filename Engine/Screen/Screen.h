#pragma once

#include <cstdint>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Shape;
struct ScreenCONST
{
    static const int VERSION_MAJOR = 4;
    static const int VERSION_MINOR = 6;
    static const int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;
    static const int16_t WIDTH = 500;
    static const int16_t HEIGHT = 1000;
};

class Screen
{
public:
    Screen();
    ~Screen();

    inline GLFWwindow* GetWindow() const {return window;}
    inline glm::mat4 GetProjection() const {return projection;}

    int8_t Move();
    
    bool CanRotate = true;
private:
    int16_t width = ScreenCONST::WIDTH;
    int16_t height = ScreenCONST::HEIGHT;
    GLFWwindow* window;
    glm::mat4 projection;
    void initialize();
    static void size_callback(GLFWwindow*, int, int);
    static void key_callback(GLFWwindow*, int, int, int, int);
};