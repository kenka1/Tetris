#include <filesystem>

#include "glad/gl.h"

#pragma once

class Program
{
public:
    Program(const std::filesystem::path&, const std::filesystem::path&);
    ~Program();
    inline GLuint GetProgram() const {return program;}
private:
    GLuint program;
    GLuint CompileShader(const std::filesystem::path&, GLenum);
    void LinkProgram(GLuint, GLuint);
    void CheckErrors(GLuint, bool);
};