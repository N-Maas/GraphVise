#include "renderer.hpp"
#include "utils.hpp"

#include <iostream>

Renderer::Renderer() : mColor{1.f, 0.55f, 0.f, 1.0f},  mShaderProgram(0),
                       vertexShaderPath(std::string(SHADERS_PATH) + std::string("/simple.vert")),
                       fragmentShaderPath(std::string(SHADERS_PATH) + std::string("/single_color.frag")),
                       mVAO(0), mVBO(0)
{
}

void Renderer::init()
{
    // Load the shader files
    reloadShaders();

    // Vertex Data -----------------------------------------------------------------------------------------------------
    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    // Make the VAO the current vertex array object by binding it
    glBindVertexArray(mVAO);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the vertex attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR();
}

void Renderer::reloadShaders()
{
    // Create shader program object and get its reference
    GLuint newProgram = utils::createShaderProgramFromFile(vertexShaderPath, fragmentShaderPath);
    if (newProgram != 0)
    {
        if (mShaderProgram != 0)
            glDeleteProgram(mShaderProgram);
        mShaderProgram = newProgram;
    }
    GL_CHECK_ERROR();
}


void Renderer::runFrame()
{
    if(mShaderProgram == 0)
    {
        std::cerr << "No shader program!" << std::endl;
        return;
    }

    // Tell OpenGL which shader program we want to use
    glUseProgram(mShaderProgram);
    GL_CHECK_ERROR();

    // Set program uniforms
    glUseProgram(mShaderProgram);
    glUniform4f(glGetUniformLocation(mShaderProgram, "color"), mColor[0], mColor[1], mColor[2], mColor[3]);
    GL_CHECK_ERROR();

    // Bind the VAO so OpenGL knows to use it
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    GL_CHECK_ERROR();
}

void Renderer::shutdown()
{
    // delete all the objects we've created
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteProgram(mShaderProgram);
}
