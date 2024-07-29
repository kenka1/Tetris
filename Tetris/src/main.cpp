#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"

int main()
{
    if(!glfwInit())
    {
        std::cerr << "ERROR glfw init" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Hello there", nullptr, nullptr);
    if(!window)
    {
        std::cerr << "ERROR glfw creation window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "ERROR load glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}