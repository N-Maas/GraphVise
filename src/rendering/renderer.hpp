//  Copyright (C) 2025, Max Piochowiak, Karlsruhe Institute of Technology
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <memory>

#include "camera.hpp"
#include "../model/GraphSaver.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "controller/RendererObserver.hpp"

class GLFWwindow;

enum LightSourceMovementBehaviour {
    FOLLOW_CAMERA,
    FIXED_POSITION
};

enum PerformanceMode {
    HIGH_PERFORMANCE,
    HIGH_RESOLUTION
};

class Renderer : public RendererObserver
{
public:
    Renderer(int framebufferWidth, int framebufferHeight);
    ~Renderer();

    // implementing methods from RendererSubject
    std::vector<std::unique_ptr<RendererObserver>> observerList;
    int signIn(RendererObserver& observer);
    int signOut(RendererObserver& observer);
    void notify();

    void init();            // Initialize all buffers, called before the main loop
    void reloadShaders();   // Reload shader programs from source files
    void runFrame(GraphSaver& graphSaver);        // Called once per Frame
    void shutdown();        // Cleanup resources, called after the main loop

    void processEvents(GLFWwindow* m_window);    // Process GLFW keyboard and mouse input
    void resize(int framebufferWidth, int framebufferHeight);

    void render(const glm::mat4& mvp, GraphSaver& graphSaver); // Render graph

    void adjustPerformanceMode(PerformanceMode performanceMode);

    // Variables to be changed in the ImGUI windows
    glm::vec4 mColor;

private:
    const float STANDARD_SPHERE_RADIUS = 0.1f;
    const float STANDARD_CYLINDER_RADIUS = 0.01f;
    float getAspectRatio() const {
    return static_cast<float>(mFramebufferSize.x) / static_cast<float>(mFramebufferSize.y);
    }

    GLuint mShaderProgram;
    // Path to shader source files
    std::string mVertexShaderPath;
    std::string mFragmentShaderPath;
    // Reference containers for the vertex array object and the vertex buffer object
    GLuint mVAO, mVBO;
    // Reference containers for the vertex array object and the vertex buffer object for edges and vetices
    GLuint vertexVAO = 0, vertexVBO = 0;
    GLuint edgeVAO = 0, edgeVBO = 0;
    // Sphere mesh data (icosphere)
    std::vector<glm::vec3> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    GLuint sphereVAO, sphereVBO, sphereEBO;
    // generate sphere mesh data(icosphere)
    float sphereRadius;
    void generateIcosphere(int subdivisions = 2);
    void renderSphere(const glm::vec3& center, float sphereRadius, const glm::vec4& color, const glm::mat4& mvp);

    // Cylinder mesh (reusable)
    std::vector<glm::vec3> cylinderVertices;
    std::vector<unsigned int> cylinderIndices;
    GLuint cylinderVAO, cylinderVBO, cylinderEBO;

    float cylinderRadius;
    void generateCylinder(int segments = 16);
    void renderCylinder(const glm::vec3& start, const glm::vec3& end, float cylinderRadius, const glm::vec4& color, const glm::mat4& mvp);


    Camera mCamera;
    CameraFocusMode cameraFocusMode;
    LightSourceMovementBehaviour lightSourceMovementBehaviour;
    PerformanceMode performanceMode;
    glm::ivec2 mFramebufferSize;

    bool mF5Pressed;

    // Vertex array for screen filling cube
    // remove this when Graph gets represented
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
