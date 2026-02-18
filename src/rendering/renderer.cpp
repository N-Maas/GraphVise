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

#include "./renderer.hpp"
#include "../utils.hpp"
#include "../model/Graph.hpp"
#include "../model/Vertex.hpp"
#include "../model/Edge.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <glm/ext/matrix_transform.hpp>

#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "controller/ButtonController.hpp"
#include "model/GraphSaver.hpp"

namespace graphvise {

    Renderer::Renderer()
    : mFramebufferSize(800, 600),  // Default size
      mShaderProgram(0),
      mVertexShaderPath(std::string(SHADERS_PATH) + std::string("graph.vert")),
     mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("graph_color.frag")),
     sphereVAO(0),
     sphereVBO(0),
     sphereEBO(0),
     sphereRadius(STANDARD_SPHERE_RADIUS),
     cylinderVAO(0),
      cylinderVBO(0),
     cylinderEBO(0),
     cylinderRadius(STANDARD_CYLINDER_RADIUS),
     mCamera(),
     lightPos({2.0f, 2.0f, 2.0f}),
     lightSourceMovementBehaviour(LightSourceMovementBehaviour::FIXED_POSITION),
     performanceMode(),
     mF5Pressed(false)
    {
        // Default quality settings
        mSettings.targetFPS = 60;
        mSettings.geometryDetail = 2;
        mSettings.cylinderSegments = 12;
    }

    Renderer::Renderer(int framebufferWidth, int framebufferHeight)
        : mFramebufferSize(framebufferWidth, framebufferHeight),
          mShaderProgram(0),
          mVertexShaderPath(std::string(SHADERS_PATH) + std::string("graph.vert")),
          mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("graph_color.frag")),
          sphereVAO(0),
          sphereVBO(0),
          sphereEBO(0),
          sphereRadius(STANDARD_SPHERE_RADIUS),
          cylinderVAO(0),
          cylinderVBO(0),
          cylinderEBO(0),
          cylinderRadius(STANDARD_CYLINDER_RADIUS),
          mCamera(),
          lightPos({2.0f, 2.0f, 2.0f}),
          lightSourceMovementBehaviour(),
          performanceMode(),
          mF5Pressed(false)
    {
        // Default quality settings
        mSettings.targetFPS = 60;
        mSettings.geometryDetail = 2;
        mSettings.cylinderSegments = 12;
    }


    Renderer::~Renderer() {
        // Cleanup OpenGL resources
        if (vertexVAO) glDeleteVertexArrays(1, &vertexVAO);
        if (vertexVBO) glDeleteBuffers(1, &vertexVBO);
        if (edgeVAO) glDeleteVertexArrays(1, &edgeVAO);
        if (sphereVAO) glDeleteBuffers(1, &sphereVAO);
        if (sphereVBO) glDeleteBuffers(1, &sphereVBO);
        if (sphereEBO) glDeleteBuffers(1, &sphereEBO);
        if (cylinderVAO) glDeleteBuffers(1, &cylinderVAO);
        if (cylinderVBO) glDeleteBuffers(1, &cylinderVBO);
        if (cylinderEBO) glDeleteBuffers(1, &cylinderEBO);
        if (mShaderProgram) glDeleteProgram(mShaderProgram);
    }

    // Singleton getters
    std::shared_ptr<Renderer> Renderer::getInstance(int framebufferWidth, int framebufferHeight) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!rendererInstance) {
            rendererInstance = std::shared_ptr<Renderer>(new Renderer(framebufferWidth, framebufferHeight),
                [](Renderer* ptr) { delete ptr; });
        } else {
            // If instance already exists, resize it
            rendererInstance->resize(framebufferWidth, framebufferHeight);
        }
        return rendererInstance;
    }

    std::shared_ptr<Renderer> Renderer::getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!rendererInstance) {
            rendererInstance = std::shared_ptr<Renderer>(
                new Renderer(),
                [](Renderer* ptr) { delete ptr; }
            );
        }
        return rendererInstance;
    }

    /**
     * Initialize the renderer and all of its (OpenGL) resources. Must be called before runFrame().
     */
    void Renderer::init()
    {
        // contains OpenGL initialization
        // Load the shader files
        loadShaders();

        if (mCamera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS) {
            mCamera.lookAtFocus();// per default camera looks at (0,0,0)
        }

        // Generate meshes if needed
        if (sphereVertices.empty()) {
            generateIcosphere(2);
        }
        if (cylinderVertices.empty()) {
            generateCylinder(12);
        }

        // Create picking framebuffer
        createPickingFramebuffer();

        //for rendering transparent objects
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GL_CHECK_ERROR();
    }

    void Renderer::createPickingFramebuffer()
    {
        // Delete old if exists
        if (pickingFramebuffer) {
            glDeleteFramebuffers(1, &pickingFramebuffer);
            glDeleteTextures(1, &pickingTexture);
            glDeleteTextures(1, &colorTexture);
        }

        // Create framebuffer
        glGenFramebuffers(1, &pickingFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer);

        // --- ATTACHMENT 0: Color texture (RGBA8) - required for location 0 output ---
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mFramebufferSize.x, mFramebufferSize.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Attach to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        // --- ATTACHMENT 1: Picking texture (R32UI) - for vertex IDs ---
        glGenTextures(1, &pickingTexture);
        glBindTexture(GL_TEXTURE_2D, pickingTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32UI, mFramebufferSize.x, mFramebufferSize.y,
                     0, GL_RG_INTEGER, GL_UNSIGNED_INT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Attach to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pickingTexture, 0);

        // --- Depth buffer (required for depth testing) ---
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mFramebufferSize.x, mFramebufferSize.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        // Set draw buffers
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffers);

        // Check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Picking framebuffer not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GL_CHECK_ERROR();
    }

    /**
     * Reloads the shaders from the file paths and compiles a new shader program to use.
     */
    void Renderer::loadShaders()
    {
        //std::cout << "=== DEBUG: Reloading Shaders ===" << std::endl;
        //std::cout << "Vertex shader path: " << mVertexShaderPath << std::endl;
        //std::cout << "Fragment shader path: " << mFragmentShaderPath << std::endl;
        // Create shader program object and get its reference
        GLuint newProgram = utils::createShaderProgramFromFile(mVertexShaderPath, mFragmentShaderPath);
        if (newProgram != 0)
        {
            if (mShaderProgram != 0)
                glDeleteProgram(mShaderProgram);
            mShaderProgram = newProgram;
        }
        GL_CHECK_ERROR();

        // Setup buffers and for visualizing graph
        glGenVertexArrays(1, &vertexVAO);
        glGenBuffers(1, &vertexVBO);
        glGenVertexArrays(1, &edgeVAO);
        glGenBuffers(1, &edgeVBO);

    }

    /**
     * Called in the main loop to render a new frame.
     */
    void Renderer::runFrame()
    {

        // --- FIRST PASS: Render to picking framebuffer (invisible) ---
        glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer);
        glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Clear to UINT32_MAX (0xFFFFFFFF)
        GLuint clearValue = 0xFFFFFFFF;
        glClearBufferuiv(GL_COLOR, 1, &clearValue);  // Clear attachment 1
        glClear(GL_DEPTH_BUFFER_BIT);

        // Enable depth testing for picking pass
        glEnable(GL_DEPTH_TEST);

        // Render with picking shader (same shader, it already outputs ID to location 1)
        if (mShaderProgram != 0) {
            glUseProgram(mShaderProgram);
            glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());
            render(mvp);
        }

        // --- SECOND PASS: Render to screen (normal rendering) ---
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // binding to default framebuffer
        glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y);
        GL_CHECK_ERROR();

        // Specify the color of the background
        glClearColor(0.f, 0.14f, 0.28f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(mShaderProgram == 0)
        {
            std::cerr << "No shader program!" << std::endl;
            return;
        }

        // MVP matrix
        glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());
        render(mvp);
        GL_CHECK_ERROR();
        notify();
    }

    // rendering Graph
    void Renderer::render(const glm::mat4& mvp) {
        //std::cout << "DEBUG: Renderer::render() called!" << std::endl;

        Graph& graph = GraphSaver::getInstance().getGraph();

        std::vector<Vertex*> vertices = graph.getVerticesSortedByTransparency();
        std::vector<Edge*> edges = graph.getEdgesSortedByTransparency();

        if (mShaderProgram == 0 || vertices.empty()) {
            std::cout << "ERROR: No shader or graph.vertices" << std::endl;
            return;
        }

        glUseProgram(mShaderProgram);

        // Set uniforms
        GLint lightPosLoc = glGetUniformLocation(mShaderProgram, "lightPos");
        GLint lightColorLoc = glGetUniformLocation(mShaderProgram, "lightColor");

        // Set lighting (use same light as cube)
        if (lightPosLoc != -1) {
            glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        }
        if (lightColorLoc != -1) {
            glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light
        }

        // Set MVP uniform
        GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
        if (mvpLoc != -1) {
            glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        } else {
            std::cout << "WARNING: No 'mvp' uniform in graph shader!" << std::endl;
        }

        // Generate meshes once (if not already)
        if (sphereVertices.empty()) {
            std::cout << "Generating sphere mesh..." << std::endl;
            generateIcosphere(2);  // Medium quality
        }
        if (cylinderVertices.empty()) {
            std::cout << "Generating cylinder mesh..." << std::endl;
            generateCylinder(12);  // 12 segments
        }

        //std::cout << "Rendering graph with " << vertices.size() << " graph.vertices and "
        //          << edges.size() << " edges" << std::endl;

        // ===== RENDER graph.vertices AS SPHERES =====
        for (const Vertex* vertex : vertices) {
            //std::cout << "iterating through vertices" << std::endl;
            renderSphere(vertex->getCoordsVector(), sphereRadius, vertex->getVec4(), mvp, vertex->getID());
        }
        // ===== RENDER EDGES AS CYLINDERS =====
        for (const auto& edge : edges) {
            int fromIdx = edge->getConnectingVerticesIDs().first;
            int toIdx = edge->getConnectingVerticesIDs().second;
            if (fromIdx < vertices.size() && toIdx < vertices.size()) {
                glm::vec3 fromPos = graph.getVertexByID(fromIdx).getCoordsVector();
                glm::vec3 toPos = graph.getVertexByID(toIdx).getCoordsVector();
                renderCylinder(fromPos, toPos,
                              cylinderRadius, edge->getVec4(), mvp, edge->getID());
            }
        }

    }

    /**
     * Frees and deletes all acquired (OpenGL) objects. Objects are freed in inverse order of how they were acquired.
     */
    void Renderer::shutdown()
    {
        // delete all the objects we've created
        glDeleteProgram(mShaderProgram);
        //delete render buffers and arrays
        glDeleteVertexArrays(1, &sphereVAO);
        glDeleteBuffers(1, &sphereVBO);
        glDeleteBuffers(1, &sphereEBO);
        glDeleteVertexArrays(1, &cylinderVAO);
        glDeleteBuffers(1, &cylinderVBO);
        glDeleteBuffers(1, &cylinderEBO);
    }

    void Renderer::resize(int framebufferWidth, int framebufferHeight)
    {
        if (framebufferWidth <= 0 || framebufferHeight <= 0) {
            std::cerr << "Invalid resize dimensions!" << std::endl;
            return;
        }
        std::cout << "Renderer::resize(" << framebufferWidth
             << ", " << framebufferHeight << ")" << std::endl;

        // Update stored size
        mFramebufferSize.x = framebufferWidth;
        mFramebufferSize.y = framebufferHeight;

        // Recreate picking framebuffer for new size
        createPickingFramebuffer();
    }

    void Renderer::generateIcosphere(int subdivisions) {
        // Icosahedron vertices (12 vertices)
        const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

        std::vector<glm::vec3> vertices = {
            {-1,  t,  0}, {1,  t,  0}, {-1, -t,  0}, {1, -t,  0},
            {0, -1,  t}, {0,  1,  t}, {0, -1, -t}, {0,  1, -t},
            {t,  0, -1}, {t,  0,  1}, {-t,  0, -1}, {-t,  0,  1}
        };

        // Normalize to unit sphere
        for (auto& v : vertices) v = glm::normalize(v);

        // Icosahedron faces (20 faces = 60 indices)
        std::vector<unsigned int> indices = {
            0,11,5, 0,5,1, 0,1,7, 0,7,10, 0,10,11,
            1,5,9, 5,11,4, 11,10,2, 10,7,6, 7,1,8,
            3,9,4, 3,4,2, 3,2,6, 3,6,8, 3,8,9,
            4,9,5, 2,4,11, 6,2,10, 8,6,7, 9,8,1
        };

        // Subdivision
        for (int i = 0; i < subdivisions; i++) {
            std::vector<unsigned int> newIndices;
            std::map<std::pair<unsigned int, unsigned int>, unsigned int> edgeVertices;

            // Subdivide each triangle into 4
            for (size_t j = 0; j < indices.size(); j += 3) {
                unsigned int v0 = indices[j];
                unsigned int v1 = indices[j+1];
                unsigned int v2 = indices[j+2];

                // Get or create edge vertices
                auto getOrCreateEdgeVertex = [&](unsigned int a, unsigned int b) {
                    if (a > b) std::swap(a, b);
                    auto key = std::make_pair(a, b);
                    if (edgeVertices.find(key) == edgeVertices.end()) {
                        glm::vec3 mid = glm::normalize(vertices[a] + vertices[b]);
                        vertices.push_back(mid);
                        edgeVertices[key] = vertices.size() - 1;
                    }
                    return edgeVertices[key];
                };

                unsigned int a = getOrCreateEdgeVertex(v0, v1);
                unsigned int b = getOrCreateEdgeVertex(v1, v2);
                unsigned int c = getOrCreateEdgeVertex(v2, v0);

                // Add 4 new triangles
                newIndices.insert(newIndices.end(), {v0, a, c});
                newIndices.insert(newIndices.end(), {v1, b, a});
                newIndices.insert(newIndices.end(), {v2, c, b});
                newIndices.insert(newIndices.end(), {a, b, c});
            }

            indices = newIndices;
        }

        sphereVertices = vertices;
        sphereIndices = indices;

        // Create OpenGL buffers
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);

        glBindVertexArray(sphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3),
                     sphereVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int),
                     sphereIndices.data(), GL_STATIC_DRAW);

        // Enable and set up vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Renderer::generateCylinder(int segments) {
        cylinderVertices.clear();
        cylinderIndices.clear();

        // Generate vertices for top and bottom circles
        float radius = 1.0f;
        float height = 1.0f;

        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            float x = cos(angle);
            float z = sin(angle);

            // Bottom vertex
            cylinderVertices.push_back({x * radius, -height/2, z * radius});
            // Top vertex
            cylinderVertices.push_back({x * radius, height/2, z * radius});
        }

        // Generate indices for sides
        for (int i = 0; i < segments; i++) {
            int bottomCurrent = i * 2;
            int topCurrent = i * 2 + 1;
            int bottomNext = (i + 1) * 2;
            int topNext = (i + 1) * 2 + 1;

            // First triangle: bottomCurrent -> topCurrent -> bottomNext
            cylinderIndices.push_back(bottomCurrent);
            cylinderIndices.push_back(topCurrent);
            cylinderIndices.push_back(bottomNext);

            // Second triangle: topCurrent -> topNext -> bottomNext
            cylinderIndices.push_back(topCurrent);
            cylinderIndices.push_back(topNext);
            cylinderIndices.push_back(bottomNext);
        }

        // Add top and bottom caps
        int centerBottom = cylinderVertices.size();
        cylinderVertices.push_back({0.0f, -height/2, 0.0f});

        int centerTop = cylinderVertices.size();
        cylinderVertices.push_back({0.0f, height/2, 0.0f});

        // Bottom cap triangles
        for (int i = 0; i < segments; i++) {
            int current = i * 2;
            int next = ((i + 1) % segments) * 2;

            cylinderIndices.push_back(centerBottom);
            cylinderIndices.push_back(current);
            cylinderIndices.push_back(next);
        }

        // Top cap triangles
        for (int i = 0; i < segments; i++) {
            int current = i * 2 + 1;
            int next = ((i + 1) % segments) * 2 + 1;

            cylinderIndices.push_back(centerTop);
            cylinderIndices.push_back(current);
            cylinderIndices.push_back(next);
        }

        // Create OpenGL buffers
        glGenVertexArrays(1, &cylinderVAO);
        glGenBuffers(1, &cylinderVBO);
        glGenBuffers(1, &cylinderEBO);

        glBindVertexArray(cylinderVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(glm::vec3),
                     cylinderVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(unsigned int),
                     cylinderIndices.data(), GL_STATIC_DRAW);

        // Enable and set up vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }


    void Renderer::renderSphere(const glm::vec3& center, float radius,
                                    const glm::vec4& color, const glm::mat4& viewProj, const uint32_t vertexId) {
        if (sphereVAO == 0) {
            std::cerr << "    ERROR: sphereVAO is 0!" << std::endl;
            return;
        }

        //debug
        //std::cout << "Rendering sphere at (" << center.x << "," << center.y << "," << center.z
        //          << ") with color (" << color.r << "," << color.g << "," << color.b << ")" << std::endl;

        // Create model matrix: translate to center, scale by radius
        glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
        model = glm::scale(model, glm::vec3(radius));

        glm::mat4 mvp = viewProj * model;

        // Set uniforms
        GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
        GLint modelLoc = glGetUniformLocation(mShaderProgram, "model");
        GLint colorLoc = glGetUniformLocation(mShaderProgram, "objectColor");
        GLint transparencyLoc = glGetUniformLocation(mShaderProgram, "transparency");
        GLint vertexIdLoc = glGetUniformLocation(mShaderProgram, "vertexId");
        GLint edgeIdLoc = glGetUniformLocation(mShaderProgram, "edgeId");

        //debug
        //std::cout << "objectColor uniform location: " << colorLoc << std::endl;
        if (colorLoc != -1) {
            glUniform3f(colorLoc, color.r, color.g, color.b);

            //std::cout << "Set color to (" << color.r << "," << color.g << "," << color.b << color.a << ")" << std::endl;
        } else {
            std::cout << "ERROR: objectColor uniform not found in shader!" << std::endl;
            // Check what uniforms actually exist
            GLint numUniforms;
            glGetProgramiv(mShaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
            std::cout << "Shader has " << numUniforms << " uniforms:" << std::endl;
            for (int i = 0; i < numUniforms; i++) {
                char name[256];
                glGetActiveUniform(mShaderProgram, i, sizeof(name), NULL, NULL, NULL, name);
                std::cout << "  " << name << std::endl;
            }
        }
        // end debug

        if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
        if (transparencyLoc != -1) glUniform1f(transparencyLoc, color.a);
        if (vertexIdLoc != -1) glUniform1ui(vertexIdLoc, vertexId);
        if (edgeIdLoc != -1) glUniform1ui(edgeIdLoc, UINT32_MAX);

        // Render
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Renderer::renderCylinder(const glm::vec3& start, const glm::vec3& end,
                                      float radius, const glm::vec4& color,
                                      const glm::mat4& viewProj, const uint32_t edgeId) const {
        glm::vec3 direction = end - start;
        float length = glm::length(direction);

        if (length < 0.001f) return;

        // Create model matrix
        glm::mat4 model = glm::mat4(1.0f);

        // Translate to midpoint
        glm::vec3 midpoint = (start + end) * 0.5f;
        model = glm::translate(model, midpoint);

        // Rotate to align with direction
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 normalizedDir = direction / length;

        // Check if direction is parallel to up (vertical)
        // If the direction is vertical, no rotation is needed, or use identity rotation
        // The cylinder's default orientation (aligned with Y axis) is already correct
        // So we can skip the rotation entirely
        const float epsilon = 0.0001f;
        if (std::abs(glm::dot(up, normalizedDir)) > 1.0f - epsilon) {

        } else {
            // For non-vertical edges, calculate rotation
            glm::vec3 axis = glm::cross(up, normalizedDir);
            axis = glm::normalize(axis); // Always normalize the axis
            float angle = acos(glm::dot(up, normalizedDir));
            model = glm::rotate(model, angle, axis);
        }
        /*
        glm::vec3 axis = glm::cross(up, direction);
        float angle = acos(glm::dot(up, direction / length));
        model = glm::rotate(model, angle, axis);
        */

        // Scale: radius in X/Z, length in Y
        model = glm::scale(model, glm::vec3(radius, length, radius));

        glm::mat4 mvp = viewProj * model;

        // Set uniforms
        GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
        GLint modelLoc = glGetUniformLocation(mShaderProgram, "model");
        GLint colorLoc = glGetUniformLocation(mShaderProgram, "objectColor");
        GLint transparencyLoc = glGetUniformLocation(mShaderProgram, "transparency");
        GLint vertexIdLoc = glGetUniformLocation(mShaderProgram, "vertexId");
        GLint edgeIdLoc = glGetUniformLocation(mShaderProgram, "edgeId");

        if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
        if (colorLoc != -1) glUniform3f(colorLoc, color.r, color.g, color.b);
        if (transparencyLoc != -1) glUniform1f(transparencyLoc, color.a);
        if (vertexIdLoc != -1) glUniform1ui(vertexIdLoc, UINT32_MAX);  // Clear vertex ID
        if (edgeIdLoc != -1) glUniform1ui(edgeIdLoc, edgeId);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Renderer::generateGeometryBasedOnQuality()
    {
        int sphereSubdivisions;

        switch (mSettings.geometryDetail) {
            case 0:  // Low quality
                sphereSubdivisions = 1;
                mSettings.cylinderSegments = 8;
                break;
            case 1:  // Medium quality
                sphereSubdivisions = 2;
                mSettings.cylinderSegments = 12;
                break;
            case 2:  // High quality
                sphereSubdivisions = 3;
                mSettings.cylinderSegments = 16;
                break;
            default:
                sphereSubdivisions = 2;
                mSettings.cylinderSegments = 12;
        }

        // Regenerate geometry
        generateIcosphere(sphereSubdivisions);
        generateCylinder(mSettings.cylinderSegments);
    }

    void Renderer::setQualityPreset(QualityPreset preset)
    {
        switch (preset) {
            case QualityPreset::LOW:
                mSettings.geometryDetail = 0;
                mSettings.targetFPS = 30;
                break;

            case QualityPreset::MEDIUM:
                mSettings.geometryDetail = 2;
                mSettings.targetFPS = 60;
                break;

            case QualityPreset::HIGH:
                mSettings.geometryDetail = 3;
                mSettings.targetFPS = 80;
                break;
        }
        // Regenerate geometry with new quality
        generateGeometryBasedOnQuality();

        // Reinitialize resources with new settings
        //init();
    }

    void Renderer::setTargetFPS(int fps)
    {
        mSettings.targetFPS = std::max(1, std::min(fps, 240)); // Clamp to reasonable range
    }

    void Renderer::setGeometryDetail(int detail)
    {
        detail = std::max(0, std::min(detail, 3)); // Clamp to 0-3
        if (mSettings.geometryDetail != detail) {
            mSettings.geometryDetail = detail;
            generateGeometryBasedOnQuality();
        }
    }

    PickedObject Renderer::getObjectAt(double x, double y)
    {
        PickedObject result;
        // Make sure coordinates are within framebuffer
        if (x < 0 || x >= mFramebufferSize.x ||
            y < 0 || y >= mFramebufferSize.y) {
            return result;  // No vertex
            }

        // Bind picking framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer);
        glReadBuffer(GL_COLOR_ATTACHMENT1);

        // Read the pixel values [vertexId, edgeId]
        uint32_t pixelValues[2];
        glReadPixels(static_cast<int>(x), static_cast<int>(y),
                     1, 1, GL_RG_INTEGER, GL_UNSIGNED_INT, pixelValues);

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Debug output to see what you're actually reading
        std::cout << "Picked values: [" << pixelValues[0] << ", " << pixelValues[1] << "]" << std::endl;

        if (pixelValues[0] != UINT32_MAX) {
            result.type = PickedObject::Type::VERTEX;
            result.id = pixelValues[0];
        } else if (pixelValues[1] != UINT32_MAX) {
            result.type = PickedObject::Type::EDGE;
            result.id = pixelValues[1];
        }
        GL_CHECK_ERROR();

        return result;
    }

    void RendererSubject::signIn(std::reference_wrapper<RendererObserver> observer) {
        this->observerList.push_back(observer);
    };

    void RendererSubject::signOut(std::reference_wrapper<RendererObserver> observer) {
        auto it = std::ranges::find_if(observerList,
                                       [observer](const std::reference_wrapper<RendererObserver> ref) {
                                           return &ref.get() == &observer.get();
                                       }
        );
        if (it != observerList.end()) {
            observerList.erase(it);
        }
    };

    void RendererSubject::notify() {
        for (const auto& observer : observerList) {
            observer.get().update();  // Call update on each observer
        }
    };

    RendererSubject::~RendererSubject() = default;
}
