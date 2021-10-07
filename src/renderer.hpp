#ifndef THESIS_FRAMEWORK_RENDERER_HPP
#define THESIS_FRAMEWORK_RENDERER_HPP

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.hpp"

class GLFWwindow;

class Renderer
{
public:
    Renderer(int framebufferWidth, int framebufferHeight);

    void init();            // Initialize all buffers, called before the main loop
    void reloadShaders();   // Reload shader programs from source files
    void runFrame();        // Called once per Frame
    void shutdown();        // Cleanup resources, called after the main loop

    void processEvents(GLFWwindow* m_window);    // Process GLFW keyboard and mouse input
    void resize(int framebufferWidth, int framebufferHeight);

    // Variables to be changed in the ImGUI windows
    glm::vec4 mColor;

private:
    float getAspectRatio() const;

    GLuint mShaderProgram;
    // Path to shader source files
    std::string mVertexShaderPath;
    std::string mFragmentShaderPath;
    // Reference containers for the vertex array object and the vertex buffer object
    GLuint mVAO, mVBO;

    Camera mCamera;
    glm::ivec2 mFramebufferSize;

    bool mF5Pressed;

    // Vertex array for screen filling quad
    static constexpr GLfloat cubeVertices[] = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };
};

#endif //THESIS_FRAMEWORK_RENDERER_HPP
