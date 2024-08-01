#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "Program/Program.h"

Program::Program(const std::filesystem::path& vert_path, const std::filesystem::path& frag_path)
{
    GLuint vert_shader = CompileShader(vert_path, GL_VERTEX_SHADER);
    GLuint frag_shader = CompileShader(frag_path, GL_FRAGMENT_SHADER);
    LinkProgram(vert_shader, frag_shader);
}

Program::~Program()
{
    glDeleteProgram(program);
}

GLuint Program::CompileShader(const std::filesystem::path& shader_path, GLenum type)
{
    std::ifstream file(shader_path);
    if(!file.is_open())
    {
        std::cerr << "can not open shader file : " << shader_path.c_str() << std::endl;
        std::abort();
    }
 
    std::string shader_str;
    shader_str.append(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const GLchar* shader_source = shader_str.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);

    CheckErrors(shader, 1);

    return shader;
}

void Program::LinkProgram(GLuint vert_shader, GLuint frag_shader)
{
    program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    CheckErrors(program, 0);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void Program::CheckErrors(GLuint id, bool flag)
{
    GLint result;
    if(flag)
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    else
        glGetProgramiv(id, GL_LINK_STATUS, &result);

    if(result == 0)
    {
        GLchar log[1000];
        if(flag)
            glGetShaderInfoLog(id, 1000, nullptr, log);
        else
            glGetProgramInfoLog(id, 1000, nullptr, log);
        std::cerr << log << std::endl;
        std::abort();
    }
}