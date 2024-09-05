#pragma once

#include <cstdint>

#include "glm/glm.hpp"
#include "glad/gl.h"
#include "PlayerState/PlayerState.h"

class Shape
{
public:
    Shape();
    ~Shape();

    void StoreData(GLsizeiptr, const void*);
    void StoreIndices(GLsizeiptr, const void*);

    inline GLuint GetVao() const {return vao;}
    inline GLuint GetVbo() const {return vbo;}
    inline glm::vec3 GetTranslate() const {return translate;}
    inline glm::mat4 GetTransform() const {return transform;}

    void Scale(float);
    void Translate(const glm::vec3&);
    void UpdateTransform();
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glm::vec3 scale;
    glm::vec3 translate;
    glm::mat4 transform;
};