//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP
#define THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP

#include "../rendering/camera.hpp"
#include "rendering/renderer.hpp"

namespace graphvise
{
    class MovementController {
    public:
        explicit MovementController() : camera(Renderer::getInstance()->m_camera()){
        }

        void moveCamera(glm::vec3 direction, bool sprinting);

        void rotateCamera(float pitchChange, float yawChange);
        void zoom(float zoomValue, bool sprinting);

    private:
        Camera& camera;
    };
}

#endif //THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP
