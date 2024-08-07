#pragma once

#include <cstdint>

#include "glm/glm.hpp"
#include "glad/gl.h"

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
    void Rotate(const glm::vec3&);
    void Translate(const glm::vec3&);
    void UpdateTransform();

    void move(int);

protected:
    u_char id;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
private:
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 translate;
    glm::mat4 transform;

    void UpdateID();
};