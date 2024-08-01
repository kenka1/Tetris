#include "glad/gl.h"

#pragma once

class Block
{
public:
    Block();
    ~Block();
    inline GLuint GetVao() const {return vao;}
private:
    GLuint vao;
    GLuint vbo;
};