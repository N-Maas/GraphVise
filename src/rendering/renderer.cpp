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

Renderer::Renderer(int framebufferWidth, int framebufferHeight)
        : mColor{1.f, 0.55f, 0.f, 1.0f}, mShaderProgram(0),
        mVertexShaderPath(std::string(SHADERS_PATH) + std::string("/graph.vert")),
        mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("/graph_color.frag")),
        mVAO(0), mVBO(0), mCamera(), mFramebufferSize(framebufferWidth, framebufferHeight), mF5Pressed(false)
{
}

Renderer::~Renderer() {
    // Cleanup OpenGL resources
    if (vertexVAO) glDeleteVertexArrays(1, &vertexVAO);
    if (vertexVBO) glDeleteBuffers(1, &vertexVBO);
    if (edgeVAO) glDeleteVertexArrays(1, &edgeVAO);
    if (edgeVBO) glDeleteBuffers(1, &edgeVBO);
    if (mShaderProgram) glDeleteProgram(mShaderProgram);
}

/**
 * Initialize the renderer and all of its (OpenGL) resources. Must be called before runFrame().
 */
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Configure the vertex attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR();
}

/**
 * Reloads the shaders from the file paths and compiles a new shader program to use.
 */
void Renderer::reloadShaders()
{
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

//todo will work properly when Model is implemented
/**
 * Called in the main loop to render a new frame.
 */
void Renderer::runFrame(GraphSaver& graphSaver)
{
    if(mShaderProgram == 0)
    {
        std::cerr << "No shader program!" << std::endl;
        return;
    }

    // Tell OpenGL which shader program we want to use
    glUseProgram(mShaderProgram);
    GL_CHECK_ERROR();

    //todo all this code is for rendering the cube and has to be thrown out when the cube is no longer needed

    // 2.1 MVP matrix
    glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());
    GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
    if (mvpLoc != -1) {
        glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        std::cout << "Set cube MVP" << std::endl;
    }

    // 2.2 Model matrix (identity for now)
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(mShaderProgram, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    }

    // 3.Set lighting uniforms for CUBE
    GLint lightPosLoc = glGetUniformLocation(mShaderProgram, "lightPos");
    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        std::cout << "Set lightPos to (2,2,2)" << std::endl;
    } else {
        std::cout << "WARNING: lightPos uniform not found in cube shader!" << std::endl;
    }

    // 4. Set cube color (ORANGE)
    GLint colorLoc = glGetUniformLocation(mShaderProgram, "objectColor");
    if (colorLoc == -1) colorLoc = glGetUniformLocation(mShaderProgram, "color");
    if (colorLoc != -1) {
        glUniform3f(colorLoc, mColor[0], mColor[1], mColor[2]);  // Orange
        std::cout << "Set cube color to orange: ("
                  << mColor[0] << "," << mColor[1] << "," << mColor[2] << ")" << std::endl;
    } else {
        std::cout << "WARNING: No color uniform found in cube shader!" << std::endl;
    }


    // todo throw test cube out when graph can be rendered
    //Render cube
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    std::cout << "Cube drawn" << std::endl;

    /* todo currently we are using the same shader program for the cube and the graph, this of course does not work and
    * todo we are only using the graph shader program
    * todo we need access to GraphSaver object
     */
    //rendering graph
    render(mvp, graphSaver);
    notify();
}

// todo will work properly when Model is implemented
// rendering Graph
void Renderer::render(const glm::mat4& mvp, GraphSaver& graphSaver) {

    //TODO: Graph is std::<optional> currently. (Greyed out, so the program runs)
    /*Graph graph = graphSaver.getGraph();
    std::vector<Vertex>& vertices = graph.getVertices();
    std::vector<Edge>& edges = graph.getEdges();
    std::cout << "=== GraphRenderer start ===" << std::endl;
    std::cout << "Graph shader ID: " << mShaderProgram << std::endl;
    std::cout << "vertex count: " << vertices.size() << std::endl;

    if (mShaderProgram == 0 || vertices.empty()) {
        std::cout << "ERROR: No shader or graph.vertices" << std::endl;
        return;
    }*/

    glUseProgram(mShaderProgram);

    // Set MVP uniform
    GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
    if (mvpLoc != -1) {
        glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        std::cout << "Set graph MVP" << std::endl;
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

    //TODO: Uncomment, when graph can be loaded
    /*
    std::cout << "Rendering graph with " << vertices.size() << " graph.vertices and "
              << edges.size() << " edges" << std::endl;

    // ===== RENDER graph.vertices AS SPHERES =====
    for (const auto& vertex : vertices) {
        renderSphere(vertex.getCoordsVector(), sphereRadius, vertex.getVertexVec4(), mvp);
    }
    // ===== RENDER EDGES AS CYLINDERS =====
    for (const auto& edge : edges) {
        int fromIdx = edge.getConnectingVerticesIDs()[0];
        int toIdx = edge.getConnectingVerticesIDs()[1];
        if (fromIdx < vertices.size() && toIdx < vertices.size()) {
            glm::vec3 fromPos = graph.getVertexByID(fromIdx).getCoordsVector();
            glm::vec3 toPos = graph.getVertexByID(toIdx).getCoordsVector();
            renderCylinder(fromPos, toPos,
                          cylinderRadius, edge.getEdgeVec4(), mvp);
        }
    }
    */
}

/**
 * Frees and deletes all acquired (OpenGL) objects. Objects are freed in inverse order of how they were acquired.
 */
void Renderer::shutdown()
{
    // delete all the objects we've created
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteProgram(mShaderProgram);
}


void Renderer::processEvents(GLFWwindow* m_window)
{
    // alternatively: use GLFW's glfwSetKeyCallback

    // F5 to reload shaders
    if (glfwGetKey(m_window, GLFW_KEY_F5) == GLFW_RELEASE)
    {
        mF5Pressed = false;
    } else
    {
        if (!mF5Pressed)
        {
            reloadShaders();
        }
        mF5Pressed = true;
    }

    // Camera Mouse
    static constexpr float PI = 3.1415926536f;
    static const float mouse_radians_per_pixel = 0.003f;
    int right_mouse_state = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2);
    double mouse_position_double[2];
    glfwGetCursorPos(m_window, &mouse_position_double[0], &mouse_position_double[1]);
    float mouse_position[2] = {(float)mouse_position_double[0], (float)mouse_position_double[1]};
    if (!mCamera.rotate_camera && right_mouse_state == GLFW_PRESS) {
        mCamera.rotate_camera = true;
        mCamera.rotation_x_0 = mCamera.rotation_x + mouse_position[1] * mouse_radians_per_pixel;
        mCamera.rotation_y_0 = mCamera.rotation_y - mouse_position[0] * mouse_radians_per_pixel;
    }
    if (right_mouse_state == GLFW_RELEASE)
        mCamera.rotate_camera = false;
    if (mCamera.rotate_camera) {
        mCamera.rotation_x = mCamera.rotation_x_0 - mouse_radians_per_pixel * mouse_position[1];
        mCamera.rotation_y = mCamera.rotation_y_0 + mouse_radians_per_pixel * mouse_position[0];
        mCamera.rotation_x = (mCamera.rotation_x < -PI) ? -PI : mCamera.rotation_x;
        mCamera.rotation_x = (mCamera.rotation_x > PI) ? PI : mCamera.rotation_x;
    }
    static double last_time = 0.0;
    double now = glfwGetTime();
    double elapsed_time = (last_time == 0.0) ? 0.0 : (now - last_time);
    auto time_delta = (float)elapsed_time;
    last_time = now;
    float final_speed = mCamera.speed;
    final_speed *= (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 10.0f : 1.0f;
    final_speed *= (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ? 0.1f : 1.0f;
    float step = time_delta * final_speed;

    // Camera Keyboard
    float forward = 0.0f, right = 0.0f, vertical = 0.0f;
    forward += (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) ? step : 0.0f;
    forward -= (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) ? step : 0.0f;
    right += (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) ? step : 0.0f;
    right -= (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) ? step : 0.0f;
    vertical += (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) ? step : 0.0f;
    vertical -= (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) ? step : 0.0f;
    float cos_y = cosf(mCamera.rotation_y), sin_y = sinf(mCamera.rotation_y);
    mCamera.position_world_space[0] +=  sin_y * forward;
    mCamera.position_world_space[0] +=  cos_y * right;
    mCamera.position_world_space[2] += -cos_y * forward;
    mCamera.position_world_space[2] +=  sin_y * right;
    mCamera.position_world_space[1] +=  vertical;
}

void Renderer::resize(int framebufferWidth, int framebufferHeight)
{
    mFramebufferSize.x = framebufferWidth;
    mFramebufferSize.y = framebufferHeight;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}


void Renderer::renderSphere(const glm::vec3& center, float radius,
                                const glm::vec4& color, const glm::mat4& viewProj) {
    if (sphereVAO == 0) return;

    //debug
    std::cout << "Rendering sphere at (" << center.x << "," << center.y << "," << center.z
              << ") with color (" << color.r << "," << color.g << "," << color.b << ")" << std::endl;

    // Create model matrix: translate to center, scale by radius
    glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
    model = glm::scale(model, glm::vec3(radius));

    glm::mat4 mvp = viewProj * model;

    glUseProgram(mShaderProgram);

    // Set uniforms
    GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
    GLint modelLoc = glGetUniformLocation(mShaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(mShaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(mShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(mShaderProgram, "lightColor");

    //debug
    std::cout << "objectColor uniform location: " << colorLoc << std::endl;
    if (colorLoc != -1) {
        glUniform3f(colorLoc, color.r, color.g, color.b);
        std::cout << "Set color to (" << color.r << "," << color.g << "," << color.b << ")" << std::endl;
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
    if (colorLoc != -1) glUniform3f(colorLoc, color.r, color.g, color.b);

    // Set lighting (use same light as cube)
    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    }
    if (lightColorLoc != -1) {
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light
    }

    // Render
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::renderCylinder(const glm::vec3& start, const glm::vec3& end,
                                  float radius, const glm::vec4& color,
                                  const glm::mat4& viewProj) const {
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
    glm::vec3 axis = glm::cross(up, direction);
    float angle = acos(glm::dot(up, direction / length));
    model = glm::rotate(model, angle, axis);

    // Scale: radius in X/Z, length in Y
    model = glm::scale(model, glm::vec3(radius, length, radius));

    glm::mat4 mvp = viewProj * model;

    // Set uniforms and render
    glUseProgram(mShaderProgram);

    // Set uniforms
    GLint mvpLoc = glGetUniformLocation(mShaderProgram, "mvp");
    GLint modelLoc = glGetUniformLocation(mShaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(mShaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(mShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(mShaderProgram, "lightColor");

    if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    if (colorLoc != -1) glUniform3f(colorLoc, color.r, color.g, color.b);


    // Set lighting (use same light as cube)
    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    }
    if (lightColorLoc != -1) {
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light
    }


    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void RendererSubject::signIn(std::shared_ptr<RendererObserver> observer) {
    this->observerList.push_back(std::move(observer));
};

void RendererSubject::signOut(std::shared_ptr<RendererObserver> observer) {;
    auto it = std::ranges::find_if(observerList,
                                   [observer](const std::shared_ptr<RendererObserver>& ptr) {
                                       return ptr.get() == observer.get();
                                   }
    );
    if (it != observerList.end()) {
        observerList.erase(it);
    }
};

void RendererSubject::notify() {
    for (const auto& observer : observerList) {
        observer->update();  // Call update on each observer
    }
};

RendererSubject::~RendererSubject() = default;