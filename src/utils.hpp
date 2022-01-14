#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

namespace utils {

    void log(const std::string&);

    void printOpenMPVersion();

    GLenum glCheckError_(const char *file, int line, bool debugBreakOnError=true);
    #define GL_CHECK_ERROR() utils::glCheckError_(__FILE__, __LINE__)

    GLuint createShader(GLenum shaderType, const GLchar **shaderSource);
    GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    GLuint createShaderProgramFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
}
