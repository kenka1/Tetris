#include "glm/glm.hpp"
#include "glad/gl.h"

#pragma once

class Shape
{
public:
    Shape();
    ~Shape();
    inline GLuint GetVao() const {return vao;}
    inline glm::vec3 GetTranslate() const {return translate;}
    inline glm::mat4 GetTransform() const {return transform;}

    void Scale(float);
    void Rotate(const glm::vec3&);
    void Translate(const glm::vec3&);
    void UpdateTransform();
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 translate;
    glm::mat4 transform;
};