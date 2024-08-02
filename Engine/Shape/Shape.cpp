#include <iostream>

#include "Shape/Shape.h"
#include "Shape/Data.h"

Shape::Shape():
    scale(1.0f), rotation(0.0f), translation(0.0f), transform(1.0f)
{
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obj::data), obj::data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glCreateBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(obj::indices), obj::indices, GL_STATIC_DRAW);
}

Shape::~Shape()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}