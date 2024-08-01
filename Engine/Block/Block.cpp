#include <iostream>

#include "Block/Block.h"
#include "Block/Data.h"

Block::Block()
{
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obj::data), obj::data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}

Block::~Block()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}