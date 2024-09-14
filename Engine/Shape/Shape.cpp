#include "Shape/Shape.h"

#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Shape::Shape():
    scale(1.0f), translate(0.0f), transform(1.0f)
{
    // std::cout << "Constructor Shape" << std::endl;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glCreateBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
} 

Shape::~Shape()
{
    std::cout << "Destructor Shape" << std::endl;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Shape::StoreData(GLsizeiptr size, const void* data)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
    glEnableVertexAttribArray(0);
}

void Shape::StoreIndices(GLsizeiptr size, const void* data)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Shape::LoadTexture(const char* image)
{
    std::filesystem::path t_path = std::filesystem::absolute(std::filesystem::current_path());
    t_path.append(image);
    std::cout << t_path.c_str() << std::endl;

    int width, height, compression;
    unsigned char *text_data = stbi_load(t_path.c_str(), &width, &height, &compression, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format = GL_RGB;
	GLenum internalformat = GL_RGB8;
	switch (compression)
	{
	case 3:
		format = GL_RGB;
		internalformat = GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		internalformat = GL_RGBA8;
		break;
	}
		
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, internalformat, width, height);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, text_data);

	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(text_data);
}

void Shape::Scale(float s)
{
    scale = glm::vec3(s);
}

void Shape::Translate(const glm::vec3& t)
{
    translate = t;
}

void Shape::UpdateTransform()
{
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, translate);
    transform = glm::scale(transform, scale);
}