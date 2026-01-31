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


#include "RendererSubject.hpp"
#include "camera.hpp"
#include "../model/GraphSaver.hpp"
#include "../controller/RendererObserver.hpp"

namespace graphvise {

    enum LightSourceMovementBehaviour {
        FOLLOW_CAMERA,
        FIXED_POSITION
    };

    enum PerformanceMode {
        HIGH_PERFORMANCE,
        HIGH_RESOLUTION
    };

    struct VertexData {
        glm::vec3 position;
        glm::vec3 normal;
    };


    class Renderer : public RendererSubject {
    public:
        // Delete copy constructor and assignment operator
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void init();            // Initialize all buffers, called before the main loop
        void reloadShaders();   // Reload shader programs from source files
        void runFrame();        // Called once per Frame
        void shutdown();        // Cleanup resources, called after the main loop

        void processEvents(GLFWwindow* m_window);    // Process GLFW keyboard and mouse input
        void resize(int framebufferWidth, int framebufferHeight);

        void render(const glm::mat4& mvp); // Render graph

        void adjustPerformanceMode(PerformanceMode newMode) {
            performanceMode = newMode;
        };

        uint32_t getVertexAt(int x, int y);// get vertex you clicked on a mose position (x,y)

        static std::shared_ptr<Renderer> getInstance();
        static std::shared_ptr<Renderer> getInstance(int framebufferWidth, int framebufferHeight);

        // Variables to be changed in the ImGUI windows
        glm::vec4 mColor;
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

    private:
        Renderer();
        Renderer(int framebufferWidth, int framebufferHeight);
        ~Renderer() override;
        // Static pointer to the Singleton instance
        static inline std::shared_ptr<Renderer> rendererInstance = nullptr;
        static inline std::mutex mtx;

        std::vector<std::shared_ptr<RendererObserver>> observerList;
        const float STANDARD_SPHERE_RADIUS = 0.05f;
        const float STANDARD_CYLINDER_RADIUS = 0.03f;
        glm::ivec2 mFramebufferSize;

        [[nodiscard]] float getAspectRatio() const {
            return static_cast<float>(mFramebufferSize.x) / static_cast<float>(mFramebufferSize.y);
        }

        GLuint framebuffer;
        GLuint colorTexture;        // Visual output (RGBA8)
        GLuint depthBuffer;

        GLuint mShaderProgram;
        // Path to shader source files
        std::string mVertexShaderPath;
        std::string mFragmentShaderPath;

        // Reference containers for the vertex array object and the vertex buffer object for edges and vetices
        GLuint vertexVAO = 0, vertexVBO = 0;
        GLuint edgeVAO = 0, edgeVBO = 0;
        // Sphere mesh data (icosphere)
        std::vector<glm::vec3> sphereVertices;
        std::vector<unsigned int> sphereIndices;
        GLuint sphereVAO{}, sphereVBO{}, sphereEBO{};
        // generate sphere mesh data(icosphere)
        float sphereRadius{};
        void generateIcosphere(int subdivisions = 2);
        void renderSphere(const glm::vec3& center, float sphereRadius, const glm::vec4& color, const glm::mat4& mvp);

        // Cylinder mesh (reusable)
        std::vector<glm::vec3> cylinderVertices;
        std::vector<unsigned int> cylinderIndices;
        GLuint cylinderVAO{}, cylinderVBO{}, cylinderEBO{};

        float cylinderRadius{};
        void generateCylinder(int segments = 16);
        void renderCylinder(const glm::vec3& start, const glm::vec3& end, float cylinderRadius, const glm::vec4& color, const glm::mat4& mvp) const;

        Camera mCamera;
        glm::vec3 lightPos;
        LightSourceMovementBehaviour lightSourceMovementBehaviour;
        PerformanceMode performanceMode;

        bool mF5Pressed;

    };
}
