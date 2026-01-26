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

#include <fstream>

#include "../utils.hpp"
#include "../model/Graph.hpp"
#include "../model/Vertex.hpp"
#include "../model/Edge.hpp"
#include "../model/GraphSaver.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <glm/ext/matrix_transform.hpp>

Renderer::Renderer(int framebufferWidth, int framebufferHeight)
    : mColor{1.f, 0.55f, 0.f, 1.0f}, mFramebufferSize(framebufferWidth, framebufferHeight),
      framebuffer(0),
      colorTexture(0),
      pickingTexture(0), depthBuffer(0),
      vertexShaderProgram(0), edgeShaderProgram(0),
      vVertexShaderPath(std::string(SHADERS_PATH) + std::string("vertex.vert")),
      vFragmentShaderPath(std::string(SHADERS_PATH) + std::string("vertex_color.frag")),
      eVertexShaderPath(std::string(SHADERS_PATH) + std::string("edge.vert")),
      eFragmentShaderPath(std::string(SHADERS_PATH) + std::string("edge_color.frag")),
      mVAO(0),mVBO(0), sphereVAO(0),
      sphereVBO(0),
      sphereEBO(0),
      sphereRadius(STANDARD_SPHERE_RADIUS),
      cylinderVAO(0),
      cylinderVBO(0), cylinderEBO(0),
      cylinderRadius(STANDARD_CYLINDER_RADIUS), mCamera(), cameraFocusMode(),
      lightSourceMovementBehaviour(), performanceMode(), mF5Pressed(false)
{
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
    if (vertexShaderProgram) glDeleteProgram(vertexShaderProgram);
    if (edgeShaderProgram) glDeleteProgram(edgeShaderProgram);
}

/**
 * Initialize the renderer and all of its (OpenGL) resources. Must be called before runFrame().
 */
void Renderer::init()
{
    // Load the shader files
    reloadShaders();


/*
    std::cout << "DEBUG: After reloadShaders():" << std::endl;
    std::cout << "  vertexShaderProgram = " << vertexShaderProgram << std::endl;
    std::cout << "  edgeShaderProgram = " << edgeShaderProgram << std::endl;


    // Check if files exist
    std::ifstream edgeVertFile(eVertexShaderPath);
    std::ifstream edgeFragFile(eFragmentShaderPath);

    if (!edgeVertFile) {
        std::cerr << "ERROR: Edge vertex shader file not found: " << eVertexShaderPath << std::endl;
    }
    if (!edgeFragFile) {
        std::cerr << "ERROR: Edge fragment shader file not found: " << eFragmentShaderPath << std::endl;
    }
    */


    // now initialize the buffers for visual rendering and the texture picking for mapping fragments to nodes
    initMultipleRenderTargets();

    // Generate meshes if needed
    if (sphereVertices.empty()) {
        generateIcosphere(2);
    }
    if (cylinderVertices.empty()) {
        generateCylinder(12);
    }
    GL_CHECK_ERROR();
}

/**
 * Reloads the shaders from the file paths and compiles a new shader program to use.
 */
void Renderer::reloadShaders()
{
    std::cout << "DEBUG: Reloading shaders..." << std::endl;

    // Load VERTEX shader (with MRT for picking)
    std::cout << "DEBUG: Loading vertex shaders:" << std::endl;
    std::cout << "  Vertex: " << vVertexShaderPath << std::endl;
    std::cout << "  Fragment: " << vFragmentShaderPath << std::endl;

    GLuint newVertexProgram = utils::createShaderProgramFromFile(vVertexShaderPath, vFragmentShaderPath);
    if (newVertexProgram != 0)
    {
        std::cout << "DEBUG: Vertex shader program created successfully with ID: " << newVertexProgram << std::endl;

        // Validate
        glValidateProgram(newVertexProgram);
        GLint validateStatus;
        glGetProgramiv(newVertexProgram, GL_VALIDATE_STATUS, &validateStatus);
        if (validateStatus == GL_FALSE) {
            std::cerr << "ERROR: Vertex shader program validation failed!" << std::endl;
            GLchar infoLog[512];
            glGetProgramInfoLog(newVertexProgram, 512, nullptr, infoLog);
            std::cerr << "Validation log: " << infoLog << std::endl;
            glDeleteProgram(newVertexProgram);
        } else {
            // Delete old and assign new
            if (vertexShaderProgram != 0) {
                glDeleteProgram(vertexShaderProgram);
            }
            vertexShaderProgram = newVertexProgram;
            std::cout << "DEBUG: Vertex shader program set to: " << vertexShaderProgram << std::endl;
        }
    } else {
        std::cerr << "ERROR: Failed to create vertex shader program!" << std::endl;
    }

    // Load EDGE shader (simple, no picking)
    std::cout << "\nDEBUG: Loading edge shaders:" << std::endl;
    std::cout << "  Vertex: " << eVertexShaderPath << std::endl;
    std::cout << "  Fragment: " << eFragmentShaderPath << std::endl;

    GLuint newEdgeProgram = utils::createShaderProgramFromFile(eVertexShaderPath, eFragmentShaderPath);
    if (newEdgeProgram != 0)
    {
        std::cout << "DEBUG: Edge shader program created successfully with ID: " << newEdgeProgram << std::endl;

        // Validate
        glValidateProgram(newEdgeProgram);
        GLint validateStatus;
        glGetProgramiv(newEdgeProgram, GL_VALIDATE_STATUS, &validateStatus);
        if (validateStatus == GL_FALSE) {
            std::cerr << "ERROR: Edge shader program validation failed!" << std::endl;
            GLchar infoLog[512];
            glGetProgramInfoLog(newEdgeProgram, 512, nullptr, infoLog);
            std::cerr << "Validation log: " << infoLog << std::endl;
            glDeleteProgram(newEdgeProgram);
        } else {
            // Delete old and assign new
            if (edgeShaderProgram != 0) {
                glDeleteProgram(edgeShaderProgram);
            }
            edgeShaderProgram = newEdgeProgram;
            std::cout << "DEBUG: Edge shader program set to: " << edgeShaderProgram << std::endl;
        }
    } else {
        std::cerr << "ERROR: Failed to create edge shader program!" << std::endl;
    }

    GL_CHECK_ERROR();
}

//todo will work properly when Model is implemented
/**
 * Called in the main loop to render a new frame.
 */
void Renderer::runFrame()
{
    std::cout << "DEBUG: runFrame started" << std::endl;//todo remove debugging messages later

    if(vertexShaderProgram == 0)
    {
        std::cerr << "No shader program!" << std::endl;
        return;
    }

    // Tell OpenGL which shader program we want to use
    glUseProgram(vertexShaderProgram);
    GL_CHECK_ERROR();


    // 2.1 MVP matrix
    glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());
    GLint mvpLoc = glGetUniformLocation(vertexShaderProgram, "mvp");
    if (mvpLoc != -1) {
        glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
        std::cout << "Set cube MVP" << std::endl;
    }


    //rendering graph
    render(mvp);
    GL_CHECK_ERROR();
    notify();
}

/**
 * Frees and deletes all acquired (OpenGL) objects. Objects are freed in inverse order of how they were acquired.
 */
void Renderer::shutdown()
{
    // todo delete all aditional buffers as well, color picking, sphere and cylinder buffers etc
    // delete all the objects we've created
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteProgram(vertexShaderProgram);
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
    initMultipleRenderTargets();
}


// todo will work properly when Model is implemented
// rendering Graph
void Renderer::render(const glm::mat4& mvp) {
    std::cout << "DEBUG: Renderer::render() called!" << std::endl;

    // creating test graph
    //todo only keep till graph in uploaded properly
    Graph graph = Graph(3);
    std::cout << "DEBUG: Graph created" << std::endl;
    /*
    graph.addVertex(0, glm::vec3(-1.0f, 0.0f, 2.0f));
    graph.addVertex(1, glm::vec3(1.0, 0.0f, 2.0f));
    graph.addVertex(2, glm::vec3(0.0f, 1.7f, 2.0f));
    */

    // Make a larger triangle
    /*
    graph.addVertex(0, glm::vec3(-3.0f, 0.0f, 5.0f));
    graph.addVertex(1, glm::vec3(3.0f, 0.0f, 5.0f));
    graph.addVertex(2, glm::vec3(0.0f, 3.0f, 5.0f));
    */
    graph.addVertex(0, glm::vec3(-1.0f, 0.0f, 2.0f));
    graph.addVertex(1, glm::vec3(1.0f, 0.0f, 2.0f));
    graph.addVertex(2, glm::vec3(0.0f, 1.0f, 2.0f));
    std::cout << "DEBUG: Added 3 vertices" << std::endl;
    graph.addEdge(0,1);
    graph.addEdge(1, 2);
    graph.addEdge(2,0);
    std::cout << "DEBUG: Added 3 edges" << std::endl;
    std::vector<std::uint32_t> myVerticeIDs = {0,1,2};
    std::vector<std::uint32_t> myEdgeIDs = {0,1,2};
    ImVec4 colorVec1= ImColor(225, 183, 25, 255);
    ImVec4 colorVec2= ImColor(225, 183, 25, 255);
    ImVec4 colorVec3= ImColor(225, 183, 25, 255);
    graph.addGroup("firstBuddies", colorVec1, {0}, myEdgeIDs);
    graph.addGroup("god help us!", colorVec2, {1}, {});
    graph.addGroup("please lets resolve this!", colorVec3, {2}, {});
    std::cout << "DEBUG: Added group" << std::endl;

    /*//todo uncomment when graph can be loaded
    GraphSaver& graphSaver = GraphSaver::getGraphSaver();
    auto& graph = graphSaver.getGraph();

    if (!graph.has_value()) {//check if graph exists
        return;
    }
    */
    //TODO: Graph is std::<optional> currently. (Greyed out, so the program runs)

    // Make sure shader is valid
    if (vertexShaderProgram == 0) {
        std::cerr << "DEBUG: No shader program for graph!" << std::endl;
        return;
    }
    // Use our MRT shader
    glUseProgram(vertexShaderProgram);

    // Check if program is actually bound:
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != (GLint)vertexShaderProgram) {
        std::cerr << "ERROR: Wrong shader bound! Current: "
                  << currentProgram << ", Expected: " << vertexShaderProgram << std::endl;
        glUseProgram(vertexShaderProgram);
    }


    // Bind our MRT framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Enable and clear depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Clear visual buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Clear picking buffer (R32UI) to 0
    GLuint clearValue = 0;
    glClearBufferuiv(GL_COLOR, 1, &clearValue);


    // Use our MRT shader
    //glUseProgram(vertexShaderProgram);

    // Set light uniforms
    GLint lightPosLoc = glGetUniformLocation(vertexShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(vertexShaderProgram, "lightColor");
    if (lightPosLoc != -1) glUniform3f(lightPosLoc, 2.0f, 2.0f, 2.0f);
    if (lightColorLoc != -1) glUniform4f(lightColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

    std::vector<Vertex>& vertices = graph.getVertices();
    std::vector<Edge>& edges = graph.getEdges();
    /*
        // Generate meshes once
        if (sphereVertices.empty()) {
            std::cout << "Generating sphere mesh..." << std::endl;
            generateIcosphere(2);  // Medium quality
        }
        if (cylinderVertices.empty()) {
            std::cout << "Generating cylinder mesh..." << std::endl;
            generateCylinder(12);  // 12 segments
        }
        */

    /* //debugging code
     * //todo get rid of it when everything works
     */
    std::cout << "=== GraphRenderer start ===" << std::endl;
    std::cout << "Graph shader ID: " << vertexShaderProgram << std::endl;


    std::cout << "vertex count: " << vertices.size() << std::endl;
    std::cout << "edge count" << edges.size() << std::endl;

    if (vertices.empty()) {
        std::cout << "DEBUG: No vertices to render!" << std::endl;
        return;
    }

    // Check VAO
    if (sphereVAO == 0) {
        std::cerr << "ERROR: sphereVAO is not initialized!" << std::endl;
        return;
    }

    // Render all vertices
    // for (const auto& vertex : vertices) {
    // In your vertex rendering loop:
    for (size_t i = 0; i < vertices.size(); i++) {
        const auto& vertex = vertices[i];
        std::cout << "\nDEBUG: Rendering vertex ID: " << vertex.getVertexID() << std::endl;
        std::cout << "  Position: (" << vertex.getCoordsVector().x << ", "
                  << vertex.getCoordsVector().y << ", " << vertex.getCoordsVector().z << ")" << std::endl;

        // Also check if positions are actually different:
        glm::vec3 pos0 = vertices[0].getCoordsVector();
        glm::vec3 pos1 = vertices[1].getCoordsVector();
        glm::vec3 pos2 = vertices[2].getCoordsVector();
        std::cout << "Distance 0->1: " << glm::distance(pos0, pos1) << std::endl;
        std::cout << "Distance 1->2: " << glm::distance(pos1, pos2) << std::endl;
        std::cout << "Distance 2->0: " << glm::distance(pos2, pos0) << std::endl;



        // Use different colors for each vertex
        ImVec4 testColors[] = {
            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),  // Red
            ImVec4(0.0f, 1.0f, 0.0f, 1.0f),  // Green
            ImVec4(0.0f, 0.0f, 1.0f, 1.0f)   // Blue
        };

        GLint objectColorLoc = glGetUniformLocation(vertexShaderProgram, "objectColor");
        if (objectColorLoc != -1) {
            ImVec4 color = testColors[i % 3];
            glUniform4f(objectColorLoc, color.x, color.y, color.z, color.w);
            std::cout << "Vertex " << i << " color: ("
                      << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
        }



    /*
    // Set object color (what user sees)
    GLint objectColorLoc = glGetUniformLocation(vertexShaderProgram, "objectColor");
    const ImVec4& color = vertex.getVertexVec4();
    std::cout << "  Color: (" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << ")" << std::endl;
    if (objectColorLoc != -1) {
        glUniform4f(objectColorLoc, color.x, color.y, color.z, color.w);
        std::cout << "  Set objectColor uniform" << std::endl;
    } else {
        std::cerr << "  ERROR: objectColor uniform not found!" << std::endl;
    }
    */

        // Set object ID for picking
        GLint objectIdLoc = glGetUniformLocation(vertexShaderProgram, "objectId");
        if (objectIdLoc != -1) {
            glUniform1ui(objectIdLoc, vertex.getVertexID());
            std::cout << "  Set objectId uniform to: " << vertex.getVertexID() << std::endl;
        } else {
            std::cerr << "  ERROR: objectId uniform not found!" << std::endl;
        }

        // Set model matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), vertex.getCoordsVector());
        float visibilityScale = 1.0f;
        model = glm::scale(model, glm::vec3(visibilityScale));
        std::cout << "  Model matrix (translation in last column):" << std::endl;
        for (int i = 0; i < 4; i++) {
            std::cout << "    ";
            for (int j = 0; j < 4; j++) {
                std::cout << model[i][j] << " ";
            }
            std::cout << std::endl;
        }
        GLint modelLoc = glGetUniformLocation(vertexShaderProgram, "model");
        if (modelLoc != -1) {
            glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
            std::cout << "  Set model uniform" << std::endl;
        } else {
            std::cerr << "  ERROR: model uniform not found!" << std::endl;
        }

        // Set MVP
        GLint mvpLoc = glGetUniformLocation(vertexShaderProgram, "mvp");
        if (mvpLoc != -1) {
            glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
            std::cout << "  Set mvp uniform" << std::endl;
        } else {
            std::cerr << "  ERROR: mvp uniform not found!" << std::endl;
        }

        // Draw sphere
        std::cout << "  Drawing sphere with VAO: " << sphereVAO
             << ", indices: " << sphereIndices.size() << std::endl;
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "  OpenGL error after draw: " << error << std::endl;
        }
    }
    std::cout << "DEBUG: Finished rendering vertices" << std::endl;

    glUseProgram(vertexShaderProgram);
    GL_CHECK_ERROR();

    //todo remove when debugging ready
    std::cout << "Rendering graph with " << vertices.size() << " graph.vertices and "
            << edges.size() << " edges" << std::endl;

    // ========== PHASE 2: RENDER EDGES TO SAME FRAMEBUFFER ==========
    // IMPORTANT: Edges render to the SAME framebuffer, but with a DIFFERENT shader
    // They only write to color attachment 0 (visual), NOT attachment 1 (picking)

    // Use EDGE shader (simple, no picking)
    if (edgeShaderProgram == 0) {  // You need to create this shader
        std::cerr << "DEBUG: No edge shader program!" << std::endl;
        return;
    }
    glUseProgram(edgeShaderProgram);

    // Set light uniforms for edge shader
    lightPosLoc = glGetUniformLocation(edgeShaderProgram, "lightPos");
    lightColorLoc = glGetUniformLocation(edgeShaderProgram, "lightColor");
    if (lightPosLoc != -1) glUniform3f(lightPosLoc, 2.0f, 2.0f, 2.0f);
    if (lightColorLoc != -1) glUniform4f(lightColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

    for (const auto& edge : edges) {
        try {
            int fromIdx = edge.getConnectingVerticesIDs()[0];
            int toIdx = edge.getConnectingVerticesIDs()[1];
            if (fromIdx < vertices.size() && toIdx < vertices.size()) {
                glm::vec3 fromPos = graph.getVertexByID(fromIdx).getCoordsVector();
                glm::vec3 toPos = graph.getVertexByID(toIdx).getCoordsVector();

                // Render cylinder with edge shader
                renderCylinder(fromPos, toPos, cylinderRadius, edge.getEdgeVec4(), mvp);
            }
        } catch (const std::exception& e) {
            std::cerr << "DEBUG: Error rendering edge: " << e.what() << std::endl;
        }
    }

    GL_CHECK_ERROR();

    // ========== PHASE 3: BLIT TO SCREEN ==========

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, mFramebufferSize.x, mFramebufferSize.y,
                      0, 0, mFramebufferSize.x, mFramebufferSize.y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERROR();

    std::cout << "DEBUG: Finished rendering graph" << std::endl;
}

uint32_t Renderer::getVertexAt(int x, int y) {
    // Convert window coordinates to framebuffer coordinates
    int fbX = x;
    int fbY = mFramebufferSize.y - y - 1;  // Flip Y

    // Bind picking framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Read from picking attachment (attachment 1)
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    // Read pixel as unsigned integer (single channel)
    GLuint pixelData;
    glReadPixels(fbX, fbY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERROR();

    return pixelData;  // 0 = background/no object
}


void Renderer::initMultipleRenderTargets() {
    // Create framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // 1. Color texture (visual, RGBA8 - for what user sees)
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mFramebufferSize.x, mFramebufferSize.y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTexture, 0);

    // 2. Picking texture (IDs, R32UI for integer precision)
    glGenTextures(1, &pickingTexture);
    glBindTexture(GL_TEXTURE_2D, pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, mFramebufferSize.x, mFramebufferSize.y, 0,
                 GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                           GL_TEXTURE_2D, pickingTexture, 0);

    // 3. Depth buffer
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mFramebufferSize.x, mFramebufferSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuffer);

    // Specify which color attachments to use
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GL_CHECK_ERROR();
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


void Renderer::renderSphere(const glm::vec3& center, float radius,
                            const glm::vec4& color, const glm::mat4& viewProj) {
    //debug
    std::cout << "Rendering sphere at (" << center.x << "," << center.y << "," << center.z
            << ") with color (" << color.r << "," << color.g << "," << color.b << ")" << std::endl;

    if (sphereVAO == 0) {
        std::cout << "DEBUG: sphereVAO is 0!" << std::endl;
        return;
    }

    // Create model matrix: translate to center, scale by radius
    glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
    model = glm::scale(model, glm::vec3(radius));
    glm::mat4 mvp = viewProj * model;

    std::cout << "DEBUG: Final sphere MVP:" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "  ";
        for (int j = 0; j < 4; j++) {
            std::cout << mvp[i][j] << " ";
        }
        std::cout << std::endl;
    }

    glUseProgram(vertexShaderProgram);

    // todo DEBUG: List all uniforms in the shader
    GLint numUniforms;
    glGetProgramiv(vertexShaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
    std::cout << "DEBUG: Shader has " << numUniforms << " uniforms:" << std::endl;

    for (int i = 0; i < numUniforms; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(vertexShaderProgram, i, sizeof(name), &length, &size, &type, name);
        std::cout << "  [" << i << "] " << name << " (type: " << type << ")" << std::endl;
    }

    // Set uniforms
    GLint mvpLoc = glGetUniformLocation(vertexShaderProgram, "mvp");
    GLint modelLoc = glGetUniformLocation(vertexShaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(vertexShaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(vertexShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(vertexShaderProgram, "lightColor");

    //debug
    std::cout << "objectColor uniform location: " << colorLoc << std::endl;
    if (colorLoc != -1) {
        glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
        std::cout << "Set color to (" << color.r << "," << color.g << "," << color.b << ")" << std::endl;
    } else {
        std::cout << "ERROR: objectColor uniform not found in shader!" << std::endl;
        // Check what uniforms actually exist
        GLint numUniforms;
        glGetProgramiv(vertexShaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
        std::cout << "Shader has " << numUniforms << " uniforms:" << std::endl;
        for (int i = 0; i < numUniforms; i++) {
            char name[256];
            glGetActiveUniform(vertexShaderProgram, i, sizeof(name), NULL, NULL, NULL, name);
            std::cout << "  " << name << std::endl;
        }
    }
    // end debug

    if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    if (colorLoc != -1) glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

    // Set lighting (use same light as cube)
    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    }
    if (lightColorLoc != -1) {
        glUniform4f(lightColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  // White light
    }

    // Render
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::generateCylinder(int segments) {
    cylinderVertices.clear();
    cylinderIndices.clear();

    float radius = 1.0f;
    float halfHeight = 0.5f;  // Cylinder goes from -0.5 to 0.5

    // Generate vertices for side faces
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cos(angle);
        float z = sin(angle);

        // Bottom vertex (y = -halfHeight)
        cylinderVertices.push_back(glm::vec3(x * radius, -halfHeight, z * radius));
        // Top vertex (y = halfHeight)
        cylinderVertices.push_back(glm::vec3(x * radius, halfHeight, z * radius));
    }

    // Generate indices for side faces (two triangles per segment)
    for (int i = 0; i < segments; i++) {
        int bottomCurrent = i * 2;
        int topCurrent = i * 2 + 1;
        int bottomNext = ((i + 1) % segments) * 2;
        int topNext = ((i + 1) % segments) * 2 + 1;

        // Triangle 1: bottomCurrent -> topCurrent -> bottomNext
        cylinderIndices.push_back(bottomCurrent);
        cylinderIndices.push_back(topCurrent);
        cylinderIndices.push_back(bottomNext);

        // Triangle 2: topCurrent -> topNext -> bottomNext
        cylinderIndices.push_back(topCurrent);
        cylinderIndices.push_back(topNext);
        cylinderIndices.push_back(bottomNext);
    }

    // Create VAO
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

    std::cout << "DEBUG: Cylinder generated with " << cylinderVertices.size()
              << " vertices and " << cylinderIndices.size() << " indices" << std::endl;
}
/*
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
*/
void Renderer::renderCylinder(const glm::vec3& start, const glm::vec3& end,
                              float radius, const glm::vec4& color,
                              const glm::mat4& viewProj) const {
     std::cout << "\nDEBUG: Rendering cylinder from ("
              << start.x << ", " << start.y << ", " << start.z << ") to ("
              << end.x << ", " << end.y << ", " << end.z << ")" << std::endl;
    std::cout << "  Length: " << glm::length(end - start) << std::endl;
    std::cout << "  Radius: " << radius << std::endl;
    glm::vec3 direction = end - start;
    float length = glm::length(direction);

    if (length < 0.001f) return;

    // Normalize direction
    glm::vec3 dirNormalized = direction / length;

    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Translate to start point
    model = glm::translate(model, start);

    // Calculate rotation to align Y-axis with direction
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 axis = glm::cross(up, dirNormalized);

    // Check if direction is parallel to up vector
    if (glm::length(axis) < 0.001f) {
        // Direction is parallel to Y-axis
        if (dirNormalized.y < 0) {
            // Pointing down, rotate 180 degrees around X
            model = glm::rotate(model, glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        // Otherwise no rotation needed (already aligned with Y)
    } else {
        float angle = acos(glm::dot(up, dirNormalized));
        model = glm::rotate(model, angle, glm::normalize(axis));
    }

    // Scale: X/Z = radius, Y = length
    model = glm::scale(model, glm::vec3(radius, length, radius));

    // Calculate MVP
    glm::mat4 mvp = viewProj * model;

    // Set uniforms
    glUseProgram(edgeShaderProgram);

    GLint mvpLoc = glGetUniformLocation(edgeShaderProgram, "mvp");
    GLint modelLoc = glGetUniformLocation(edgeShaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(edgeShaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(edgeShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(edgeShaderProgram, "lightColor");

    if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    if (colorLoc != -1) glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    }
    if (lightColorLoc != -1) {
        glUniform4f(lightColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Draw cylinder
    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
/*
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
    glUseProgram(edgeShaderProgram);

    // Set uniforms
    GLint mvpLoc = glGetUniformLocation(edgeShaderProgram, "mvp");
    GLint modelLoc = glGetUniformLocation(edgeShaderProgram, "model");
    GLint colorLoc = glGetUniformLocation(edgeShaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(edgeShaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(edgeShaderProgram, "lightColor");

    if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, false, &mvp[0][0]);
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, false, &model[0][0]);
    if (colorLoc != -1) glUniform4f(colorLoc, color.r, color.g, color.b, color.a);


    // Set lighting (use same light as cube)
    if (lightPosLoc != -1) {
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    }
    if (lightColorLoc != -1) {
        glUniform4f(lightColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  // White light
    }


    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
*/

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