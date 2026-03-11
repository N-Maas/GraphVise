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
#include "model/GraphSaver.hpp"

namespace graphvise {

    Renderer::Renderer()
    : backgroundColor(0.0f, 0.234f, 0.314f, 1.000f)  // First in class after public section
    , lightSourceMovementBehaviour(LightSourceMovementBehaviour::FIXED_POSITION)
    , performanceMode(PerformanceMode::BALANCE)  // or whatever default you want
    , sphereRadius(STANDARD_SPHERE_RADIUS)
    , cylinderRadius(STANDARD_CYLINDER_RADIUS)
    , mSettings()  // Default initialize
    , mFrameTime(0.0f)
    , mAccumulatedTime(0.0f)
    , mFrameCounter(0)
    , mFramebufferSize(800, 600)
    , mShaderProgram(0)
    , mVertexShaderPath(std::string(SHADERS_PATH) + std::string("graph.vert"))
    , mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("graph_color.frag"))
    , vertexVAO(0)
    , vertexVBO(0)
    , edgeVAO(0)
    , edgeVBO(0)
    , sphereVAO(0)
    , sphereVBO(0)
    , sphereEBO(0)
    , cylinderVAO(0)
    , cylinderVBO(0)
    , cylinderEBO(0)
    , mCamera()
    , lightPos(2.0f, 2.0f, 2.0f)
    , mF5Pressed(false)
    , pickingFramebuffer(0)
    , pickingTexture(0)
    , colorTexture(0)
    , vertexInstanceVBO(0)
    , vertexColorVBO(0)
    , vertexIdVBO(0)
    , edgeInstanceVBO(0)
    , renderingSpheres(true)  // or false, whatever your default is
    , numShaderInputs(10)
    , numSphereShaderInputs(5)
    {
        // Constructor body
        mSettings.targetFPS = 60;
        mSettings.sphereSubdiv = 2;
        mSettings.cylinderSegments = 12;
    }

    Renderer::Renderer(int framebufferWidth, int framebufferHeight)
    : backgroundColor(0.0f, 0.234f, 0.314f, 1.000f)
    , lightSourceMovementBehaviour(LightSourceMovementBehaviour::FIXED_POSITION)
    , performanceMode(PerformanceMode::BALANCE)
    , sphereRadius(STANDARD_SPHERE_RADIUS)
    , cylinderRadius(STANDARD_CYLINDER_RADIUS)
    , mSettings()
    , mFrameTime(0.0f)
    , mAccumulatedTime(0.0f)
    , mFrameCounter(0)
    , mFramebufferSize(framebufferWidth, framebufferHeight)  // Use parameters
    , mShaderProgram(0)
    , mVertexShaderPath(std::string(SHADERS_PATH) + std::string("graph.vert"))
    , mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("graph_color.frag"))
    , vertexVAO(0)
    , vertexVBO(0)
    , edgeVAO(0)
    , edgeVBO(0)
    , sphereVAO(0)
    , sphereVBO(0)
    , sphereEBO(0)
    , cylinderVAO(0)
    , cylinderVBO(0)
    , cylinderEBO(0)
    , mCamera()
    , lightPos(2.0f, 2.0f, 2.0f)
    , mF5Pressed(false)
    , pickingFramebuffer(0)
    , pickingTexture(0)
    , colorTexture(0)
    , vertexInstanceVBO(0)
    , vertexColorVBO(0)
    , vertexIdVBO(0)
    , edgeInstanceVBO(0)
    , renderingSpheres(true)
    , numShaderInputs(10)
    , numSphereShaderInputs(5)
    {
        // Constructor body
        mSettings.targetFPS = 60;
        mSettings.sphereSubdiv = 2;
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
        if (vertexInstanceVBO) glDeleteBuffers(1, &vertexInstanceVBO);
        if (vertexColorVBO) glDeleteBuffers(1, &vertexColorVBO);
        if (vertexIdVBO) glDeleteBuffers(1, &vertexIdVBO);
        if (edgeInstanceVBO) glDeleteBuffers(1, &edgeInstanceVBO);
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

        if (mShaderProgram == 0) {
            std::cerr << "ERROR: Shader program is 0 after loadShaders()!" << std::endl;
        }

        if (*mCamera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS) {
            mCamera.lookAtFocus();// per default camera looks at (0,0,0)
        }

        // Generate meshes if needed
        if (sphereVertices.empty()) {
            generateIcosphere(mSettings.sphereSubdiv);
        }
        if (cylinderVertices.empty()) {
            generateCylinder(mSettings.cylinderSegments);
        }

        // Setup buffers for visualizing graph
        glGenVertexArrays(1, &vertexVAO);
        glGenBuffers(1, &vertexVBO);
        glGenVertexArrays(1, &edgeVAO);
        glGenBuffers(1, &edgeVBO);

        if (vertexInstanceVBO == 0) {
            glGenBuffers(1, &vertexInstanceVBO);
            glGenBuffers(1, &vertexColorVBO);
            glGenBuffers(1, &vertexIdVBO);
        }
        if (edgeInstanceVBO == 0) {
            glGenBuffers(1, &edgeInstanceVBO);
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
    void Renderer::loadShaders() {
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
    }

    /**
     * Called in the main loop to render a new frame.
     */
    void Renderer::runFrame() {
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
            render();
        }
        GL_CHECK_ERROR();

        // --- SECOND PASS: Render to screen (normal rendering) ---
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // binding to default framebuffer
        glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y);
        GL_CHECK_ERROR();

        // Specify the color of the background
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(mShaderProgram == 0)
        {
            std::cerr << "No shader program!" << std::endl;
            return;
        }

        render();
        GL_CHECK_ERROR();
        notify();
    }

    // rendering Graph
    void Renderer::render() {
        //std::cout << "DEBUG: Renderer::render() called!" << std::endl;
        // Check that instance VBOs are initialized
        if (vertexInstanceVBO == 0 || vertexColorVBO == 0 ||
            edgeInstanceVBO == 0 ) {
            std::cerr << "ERROR: Instance VBOs not initialized!" << std::endl;
            return;
            }

        Graph& graph = GraphSaver::getInstance().getGraph();

        std::vector<Vertex*> vertices = graph.getVerticesSortedByTransparency();
        std::vector<Edge*> edges = graph.getEdgesSortedByTransparency();

        // MVP matrix
        glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());

        if (mShaderProgram == 0 || vertices.empty()) {
            std::cout << "ERROR: No shader or graph.vertices" << std::endl;
            return;
        }

        glUseProgram(mShaderProgram);

        // Get uniform locations
        GLuint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
        GLuint lightPosLoc = glGetUniformLocation(mShaderProgram, "lightPos");
        GLuint lightColorLoc = glGetUniformLocation(mShaderProgram, "lightColor");
        GLuint sphereRadiusLoc = glGetUniformLocation(mShaderProgram, "sphereRadius");
        GLuint cylinderRadiusLoc = glGetUniformLocation(mShaderProgram, "cylinderRadius");
        GLuint renderingSpheresLoc = glGetUniformLocation(mShaderProgram, "renderingSpheres");

        // Set lighting (use same light as cube)
        if (lightPosLoc != -1) {
            glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        }
        if (lightColorLoc != -1) {
            glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light
        }
        if (mvpLoc != -1) {
            glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        } else {
            std::cout << "WARNING: No 'mvp' uniform in graph shader!" << std::endl;
        }
        if (sphereRadiusLoc != -1) glUniform1f(sphereRadiusLoc, sphereRadius);
        if (cylinderRadiusLoc != -1) glUniform1f(cylinderRadiusLoc, cylinderRadius);

        // ===== RENDER SPHERES (instanced) =====
        if (!vertices.empty()) {
            // SAFETY CHECK: Make sure vertices vector is valid
            if (vertices.size() == 0) {
                std::cerr << "ERROR: vertices vector is empty but size check passed?" << std::endl;
                return;
            }
            // Force reset vertex attribute state
            glBindVertexArray(0);  // Unbind any VAO
            glBindVertexArray(sphereVAO);  // Rebind sphere VAO

            // Ensure sphere-specific attributes are enabled
            for (int i = 0; i < numSphereShaderInputs; i++) {
                glEnableVertexAttribArray(i);
            }
            // Disable cylinder-specific attributes
            for (int i = numSphereShaderInputs; i < numShaderInputs; i++) {
                glDisableVertexAttribArray(i);
            }

            // Only rebuild instance data if it's empty or size doesn't match
            if (vertexInstanceData.size() != vertices.size()) {
                // Rebuild instance data
                vertexInstanceData.resize(vertices.size());
                vertexColorData.resize(vertices.size());
            }
            vertexIdData.resize(vertices.size());       // separate ID buffer

            for (size_t i = 0; i < vertices.size(); i++) {
                const Vertex* vertex = vertices[i];
                if (!vertex) {
                std::cerr << "ERROR: Null vertex pointer at index " << i << std::endl;
                continue;
                }
                glm::vec3 pos = vertex->getCoordsVector();
                // SAFETY CHECK: Check for NaN or invalid values
                if (std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z)) {
                   std::cerr << "WARNING: Vertex " << i << " has NaN position" << std::endl;
                   pos = glm::vec3(0.0f);
                }

                auto group = graph.getGroupByID(vertex->getConnectedGroupID());
                glm::vec4 color = group.getVec4();
                uint32_t id = vertex->getID();
                float idAsFloat;

                // SAFETY CHECK: This memcpy is line 478 - ensure id is valid
                memcpy(&idAsFloat, &id, sizeof(uint32_t));

                vertexInstanceData[i] = glm::vec3(pos.x, pos.y, pos.z);
                vertexColorData[i] = color;
                vertexIdData[i] = id;
            }
            // SAFETY CHECK: Verify buffers exist before uploading
            if (vertexInstanceVBO == 0 || vertexColorVBO == 0) {
                std::cerr << "ERROR: Vertex VBOs not initialized!" << std::endl;
                return;
            }

            // Upload instance data
            glBindBuffer(GL_ARRAY_BUFFER, vertexInstanceVBO);
            glBufferData(GL_ARRAY_BUFFER, vertexInstanceData.size() * sizeof(glm::vec3),
                        vertexInstanceData.data(), GL_DYNAMIC_DRAW);

            // Check for OpenGL errors after upload
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL error after vertex instance upload: " << err << std::endl;
            }

            glBindBuffer(GL_ARRAY_BUFFER, vertexColorVBO);
            glBufferData(GL_ARRAY_BUFFER, vertexColorData.size() * sizeof(glm::vec4),
                        vertexColorData.data(), GL_DYNAMIC_DRAW);

            err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL error after vertex color upload: " << err << std::endl;
            }

            // Upload ID buffer
            glBindBuffer(GL_ARRAY_BUFFER, vertexIdVBO);
            glBufferData(GL_ARRAY_BUFFER, vertexIdData.size() * sizeof(uint32_t),
                         vertexIdData.data(), GL_DYNAMIC_DRAW);

            // Draw all spheres with one call
            if (renderingSpheresLoc != -1) glUniform1i(renderingSpheresLoc, 1);

            glBindVertexArray(sphereVAO);
            // SAFETY CHECK: Verify sphere indices are valid
            if (sphereIndices.empty()) {
                std::cerr << "ERROR: sphereIndices is empty!" << std::endl;
                return;
            }
            glDrawElementsInstanced(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0, vertices.size());
            err = glGetError();
            if (err != GL_NO_ERROR) {
               std::cerr << "OpenGL error after sphere draw: " << err << std::endl;
                }
            GL_CHECK_ERROR();
        }
        GL_CHECK_ERROR();


        // ===== RENDER CYLINDERS (instanced) =====
        if (!edges.empty()) {

            glBindVertexArray(0);
            glBindVertexArray(cylinderVAO);

            // Enable cylinder attributes
            for (int i = 0; i < numShaderInputs; i++) {
                glEnableVertexAttribArray(i);
            }
            // Prepare instance data
            std::vector<EdgeInstanceData> edgeData(edges.size());

            for (size_t i = 0; i < edges.size(); i++) {
                const Edge* edge = edges[i];
                auto group = graph.getGroupByID(edge->getConnectedGroupID());

                edgeData[i].matrix = edge->getMatrix();  // Already has translation + rotation
                edgeData[i].color = group.getVec4();
                edgeData[i].id = edge->getID();
            }

            // Upload all data in one buffer, interleaved data
            glBindBuffer(GL_ARRAY_BUFFER, edgeInstanceVBO);
            glBufferData(GL_ARRAY_BUFFER, edgeData.size() * sizeof(EdgeInstanceData),
                         edgeData.data(), GL_DYNAMIC_DRAW);

            if (renderingSpheresLoc != -1) glUniform1i(renderingSpheresLoc, 0);
            // Draw all cylinders with one call
            glBindVertexArray(cylinderVAO);
            glDrawElementsInstanced(GL_TRIANGLES, cylinderIndices.size(),
                                   GL_UNSIGNED_INT, 0, edges.size());
            GL_CHECK_ERROR();
            }

        glBindVertexArray(0);
        GL_CHECK_ERROR();
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
        //std::cout << "Renderer::resize(" << framebufferWidth << ", " << framebufferHeight << ")" << std::endl;

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
        // Create instance VBOs
        glGenBuffers(1, &vertexInstanceVBO);
        glGenBuffers(1, &vertexColorVBO);
        glGenBuffers(1, &vertexIdVBO);

        // setting up attributes for instance rendering
        glBindVertexArray(sphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3),
                     sphereVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int),
                     sphereIndices.data(), GL_STATIC_DRAW);

        // Enable and set up vertex attributes
        // pre-vertex attributes position aPos (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
        // pre-vertex attributes orientation aNormal (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Position (location 2)
        glBindBuffer(GL_ARRAY_BUFFER, vertexInstanceVBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);  // First 3 floats
        glVertexAttribDivisor(2, 1);
        // Color (location 3)
        glBindBuffer(GL_ARRAY_BUFFER, vertexColorVBO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
        glVertexAttribDivisor(3, 1);
        // ID (location 4)
        glBindBuffer(GL_ARRAY_BUFFER, vertexIdVBO);
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(uint32_t), 0);
        glVertexAttribDivisor(4, 1);

        // Explicitly disable attributes 5-8 that belong to cylinders
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
        glDisableVertexAttribArray(7);
        glDisableVertexAttribArray(8);

        glBindVertexArray(0);
        GL_CHECK_ERROR();
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
        // instance rendering buffer creation
        glGenBuffers(1, &edgeInstanceVBO);

        // Setup instanced attributes for cylinders
        glBindVertexArray(cylinderVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(glm::vec3),
                     cylinderVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(unsigned int),
                     cylinderIndices.data(), GL_STATIC_DRAW);

        // Enable and set up vertex attributes
        // pre-vertex attributes position aPos (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
        // pre-vertex attributes orientation aNormal (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // explicitly disable attributes 2-4 that belong to spheres
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);

        glBindBuffer(GL_ARRAY_BUFFER, edgeInstanceVBO);

        // Matrix column 0 (location 5)
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(EdgeInstanceData), (void*)offsetof(EdgeInstanceData, matrix));
        glVertexAttribDivisor(5, 1);

        // Matrix column 1 (location 6)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(EdgeInstanceData), (void*)(offsetof(EdgeInstanceData, matrix) + sizeof(glm::vec4)));
        glVertexAttribDivisor(6, 1);

        // Matrix column 2 (location 7)
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(EdgeInstanceData), (void*)(offsetof(EdgeInstanceData, matrix) + 2*sizeof(glm::vec4)));
        glVertexAttribDivisor(7, 1);

        // Matrix column 3 (location 8)
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(EdgeInstanceData), (void*)(offsetof(EdgeInstanceData, matrix) + 3*sizeof(glm::vec4)));
        glVertexAttribDivisor(8, 1);

        // Color (location 9)
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(EdgeInstanceData),  (void*)offsetof(EdgeInstanceData, color));
        glVertexAttribDivisor(9, 1);

        // edgeId (location 8)
        glEnableVertexAttribArray(10);
        glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, sizeof(EdgeInstanceData), (void*)offsetof(EdgeInstanceData, id));
        glVertexAttribDivisor(10, 1);

        glBindVertexArray(0);
        GL_CHECK_ERROR();
    }

    void Renderer::setPerformanceMode(PerformanceMode mode)
    {
        performanceMode = mode;
        switch (performanceMode) {
            case PerformanceMode::PERFORMANCE:
                mSettings.sphereSubdiv = 1;
                mSettings.cylinderSegments = 8;
                mSettings.targetFPS = 30;
                break;

            case PerformanceMode::BALANCE:
                mSettings.sphereSubdiv = 2;
                mSettings.cylinderSegments = 12;
                mSettings.targetFPS = 60;
                break;

            case PerformanceMode::QUALITY:
                mSettings.sphereSubdiv = 3;
                mSettings.cylinderSegments = 16;
                mSettings.targetFPS = 80;
                break;
            default:
            throw std::invalid_argument("Invalid performance mode");
        }
        // Regenerate geometry
        generateIcosphere(mSettings.sphereSubdiv);
        generateCylinder(mSettings.cylinderSegments);

        // Reinitialize resources with new settings
        //init();
    }

    void Renderer::setTargetFPS(int fps)
    {
        mSettings.targetFPS = std::max(1, std::min(fps, 240)); // Clamp to reasonable range
    }

    PickedObject Renderer::getObjectAt(double x, double y)
    {
        PickedObject result;
        // Make sure coordinates are within framebuffer
        if (x < 0 || x >= mFramebufferSize.x ||
            y < 0 || y >= mFramebufferSize.y) {
            std::cout << "Pick: Coordinates out of range: (" << x << ", " << y << ")" << std::endl;
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

        if (pixelValues[0] != UINT32_MAX) {
            result.type = ObjType::VERTEX;
            result.id = pixelValues[0];
        } else if (pixelValues[1] != UINT32_MAX) {
            result.type = ObjType::EDGE;
            result.id = pixelValues[1];
        }
        GL_CHECK_ERROR();

        return result;
    }

    void Renderer::clearInstanceData() {
		// Clear any existing instance data to force regeneration
		vertexInstanceData.clear();
		vertexColorData.clear();
		edgeData.clear();
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
