#include "glm/glm.hpp"
#include "glad/gl.h"

#pragma once

class Shape
{
public:
    Shape();
    ~Shape();
    inline GLuint GetVao() const {return vao;}
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 translation;
    glm::mat4 transform;
};