#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

#include "Shape/Shape.h"
#include "Shape/Data.h"

Shape::Shape():
    scale(1.0f), rotation(0.0f), translate(0.0f), transform(1.0f)
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

void Shape::Scale(float s)
{
    scale = glm::vec3(s);
}

void Shape::Rotate(const glm::vec3& r)
{
    rotation = glm::vec3(r);
}

void Shape::Translate(const glm::vec3& t)
{
    translate = t;
}

void Shape::UpdateTransform()
{
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, translate);
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, scale);
}

void Shape::move(int call)
{
    float step = 25.0f;
    glm::vec3 pos = translate;
    switch(call)
    {
    case 0:
        pos.x += step;
        Translate(pos);
        break;
    case 1:
        pos.x -= step;
        Translate(pos);
        break;
    case 2:
        pos.y -= step;
        Translate(pos);
        break;
    }
}