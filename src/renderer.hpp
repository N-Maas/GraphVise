#ifndef THESIS_FRAMEWORK_RENDERER_HPP
#define THESIS_FRAMEWORK_RENDERER_HPP

#include <string>
#include <glad/glad.h>

class Renderer
{
public:
    Renderer();

    void init();            // Initialize all buffers, called before the main loop
    void reloadShaders();   // Reload shader programs from source files
    void runFrame();        // Called once per Frame
    void shutdown();        // Cleanup resources, called after the main loop

    // Variables to be changed in the ImGUI windows
    float mColor[4];

private:
    GLuint mShaderProgram;
    // Path to shader source files
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    // Reference containers for the vertex array object and the vertex buffer object
    GLuint mVAO, mVBO;

    // Vertex array for screen filling quad
    static constexpr GLfloat vertices[] =
            {
                    -0.5f, -0.5f, 0.0f, // Lower left corner
                    0.5f, -0.5f, 0.0f, // Lower right corner
                    0.0f, 0.5f, 0.0f // Upper corner
            };
};

#endif //THESIS_FRAMEWORK_RENDERER_HPP
