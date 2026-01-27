//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP
#define THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP

#include "../rendering/camera.hpp"

namespace graphvise {
    class MovementController {
    public:
        explicit MovementController(Camera& camera);
        void moveCamera(glm::vec3 direction);
        void rotateCamera(float pitchChange, float yawChange);
        void zoom(float value);

    private:
        Camera& camera;
    };
}

#endif //THESIS_FRAMEWORK_MOVEMENTCONTROLLER_HPP