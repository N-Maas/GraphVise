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
    Renderer();

    void init();            // Initialize all buffers, called before the main loop
    void reloadShaders();   // Reload shader programs from source files
    void runFrame();        // Called once per Frame
    void shutdown();        // Cleanup resources, called after the main loop
    void updateCamera(GLFWwindow* m_window);    // Process GLFW keyboard and mouse input for the camera

    // Variables to be changed in the ImGUI windows
    glm::vec4 mColor;

private:
    GLuint mShaderProgram;
    // Path to shader source files
    std::string mVertexShaderPath;
    std::string mFragmentShaderPath;
    // Reference containers for the vertex array object and the vertex buffer object
    GLuint mVAO, mVBO;

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

    Camera mCamera;
};

#endif //THESIS_FRAMEWORK_RENDERER_HPP
