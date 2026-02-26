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


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/ext/matrix_clip_space.hpp>
#include "RendererSubject.hpp"
#include "camera.hpp"
#include "../controller/RendererObserver.hpp"

namespace graphvise {
    struct EdgeInstanceData {
        glm::mat4 matrix;  // Pre-calculated edge orientation matrix from edge->getMatrix()
        glm::vec4 color;
        uint32_t id;
    };

    struct RenderSettings {
        int targetFPS = 60;           // Target frames per second
        int sphereSubdiv = 2;         // Sphere subdivisions
        int cylinderSegments = 12;    // Cylinder segments
    };

    //representing the picked object
    struct PickedObject {
        uint32_t id = 0;                    // The ID (vertex ID or edge ID)
        enum class Type { NONE, VERTEX, EDGE } type = Type::NONE;

        bool isValid() const { return type != Type::NONE; }
        bool isVertex() const { return type == Type::VERTEX; }
        bool isEdge() const { return type == Type::EDGE; }
    };

    class Renderer : public RendererSubject {
    public:
        // Delete copy constructor and assignment operator
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void init();            // Initialize all buffers, called before the main loop
        void createPickingFramebuffer();

        void loadShaders();   // Load shader programs from source files
        void runFrame();        // Called once per Frame
        //void runFrame(float deltaTime); //remove

        void shutdown();        // Cleanup resources, called after the main loop

        void resize(int framebufferWidth, int framebufferHeight);
        void render(); // Render graph

        // Quality settings
        void setPerformanceMode(PerformanceMode mode);
        void setTargetFPS(int fps);

        const RenderSettings &getSettings() const;

        void adjustPerformanceMode(PerformanceMode newMode) {
            performanceMode = newMode;
        };

        PickedObject getObjectAt(double x, double y);// get vertexID you clicked on a mose position (x,y)

        static std::shared_ptr<Renderer> getInstance();
        static std::shared_ptr<Renderer> getInstance(int framebufferWidth, int framebufferHeight);

        // Variables to be changed in the ImGUI windows
        [[nodiscard]] Camera& m_camera() {
            return mCamera;
        }
        void setLightPos(const glm::vec3 &light_pos) {
            lightPos = light_pos;
        }

        [[nodiscard]] LightSourceMovementBehaviour light_source_movement_behaviour() const {
            return lightSourceMovementBehaviour;
        }

        void set_light_source_movement_behaviour(LightSourceMovementBehaviour light_source_movement_behaviour) {
            lightSourceMovementBehaviour = light_source_movement_behaviour;
        }

        [[nodiscard]] PerformanceMode performance_mode() const {
            return performanceMode;
        }

        void set_performance_mode(PerformanceMode performance_mode) {
            performanceMode = performance_mode;
        }

        void setCylinderRadius(const float radius) {
            cylinderRadius = radius;
        }
        void setSphereRadius(const float radius)
        {
            sphereRadius = radius;
        }

        float getSphereRadius()
        {
            return sphereRadius;
        }

        float getCylinderRadius()
        {
            return cylinderRadius;
        }

        int getFramebufferWidth() {
            return mFramebufferSize.x;
        }
        int getFramebufferHeight() {
            return mFramebufferSize.y;
        }

    private:
        //variables for render quality settings
        RenderSettings mSettings;
        float mFrameTime = 0.0f;
        float mAccumulatedTime = 0.0f;
        int mFrameCounter = 0;

        Renderer();
        Renderer(int framebufferWidth, int framebufferHeight);

        ~Renderer() override;
        // Static pointer to the Singleton instance
        static inline std::shared_ptr<Renderer> rendererInstance = nullptr;
        static inline std::mutex mtx;

        std::vector<std::reference_wrapper<RendererObserver>> observerList;
        const float STANDARD_SPHERE_RADIUS = 0.05f;
        const float STANDARD_CYLINDER_RADIUS = 0.03f;
        glm::ivec2 mFramebufferSize;

        [[nodiscard]] float getAspectRatio() const {
            return static_cast<float>(mFramebufferSize.x) / static_cast<float>(mFramebufferSize.y);
        }

        const glm::vec3 centerCoordinates = glm::vec3(0.0f, 0.0f, 0.0f);

        GLuint mShaderProgram;
        // Path to shader source files
        std::string mVertexShaderPath;
        std::string mFragmentShaderPath;

        // Reference containers for the vertex array object and the vertex buffer object for edges and vertices
        GLuint vertexVAO = 0, vertexVBO = 0;
        GLuint edgeVAO = 0, edgeVBO = 0;
        // Sphere mesh data (icosphere)
        std::vector<glm::vec3> sphereVertices;
        std::vector<unsigned int> sphereIndices;
        GLuint sphereVAO{}, sphereVBO{}, sphereEBO{};
        // generate sphere mesh data(icosphere)
        float sphereRadius{};
        void generateIcosphere(int subdivisions = 2);
        void renderSphere(const glm::vec3& center, float sphereRadius, const glm::vec4& color, const glm::mat4& mvp, const uint32_t vertexID);

        // Cylinder mesh (reusable)
        std::vector<glm::vec3> cylinderVertices;
        std::vector<unsigned int> cylinderIndices;
        GLuint cylinderVAO{}, cylinderVBO{}, cylinderEBO{};

        float cylinderRadius{};
        void generateCylinder(int segments = 16);
        void renderCylinder(const glm::vec3 & start, const glm::vec3 & end, float radius, const glm::vec4 & color, const glm::mat4 & viewProj, uint32_t
                            edgeId) const;

        Camera mCamera;
        glm::vec3 lightPos;
        LightSourceMovementBehaviour lightSourceMovementBehaviour;
        PerformanceMode performanceMode;

        bool mF5Pressed;

        // Picking framebuffer
        GLuint pickingFramebuffer = 0;
        GLuint pickingTexture = 0;  // Texture to store the IDs
        GLuint colorTexture = 0; // basic texture for visual vertex color

        // buffers needed for instance rendering
        std::vector<glm::vec3> vertexInstanceData;  // Packed: xyz=position, w=id
        std::vector<glm::vec4> vertexColorData;     // rgba colors
        std::vector<uint32_t> vertexIdData;
        std::vector<glm::vec3> edgeData;    // xyz=start, w=radius, then xyz=end, w=id
        GLuint vertexInstanceVBO;
        GLuint vertexColorVBO;
        GLuint vertexIdVBO;
        GLuint edgeInstanceVBO;
        bool renderingSpheres;  // spheres and cylinders rendered with different normal calculation in shader
        std::vector<EdgeInstanceData> edgeInterleavedData;
        uint8_t numShaderInputs = 10;
        uint8_t numSphereShaderInputs = 5;
    };
}
