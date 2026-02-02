//
// Created by yannik on 14.01.26.
//

#include "MovementController.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

#define SPRINT_INCREASE 10
#define ROTATION_RADIANS_PER_PIXEL 0.003f

namespace graphvise {

    //Using a 3D vector as direction
    //X Coordinate: Left/Right
    //Y Coordinate: Up/Down
    //Z Coordinate: Forwards/Backwards
    void MovementController::moveCamera(glm::vec3 direction, bool sprinting) {
        if (camera.camera_focus_mode() == FREE) {
            static double last_time = 0.0;
            double now = glfwGetTime();
            double elapsed_time = (last_time == 0.0) ? 0.0 : (now - last_time);
            auto time_delta = (float)elapsed_time;
            last_time = now;
            float step = time_delta * camera.speed;
            step *= sprinting ? SPRINT_INCREASE : 1.0f;

            float cos_y = cosf(camera.rotation_y), sin_y = sinf(camera.rotation_y);

            camera.position_world_space[0] +=  sin_y * direction.z * step;
            camera.position_world_space[0] +=  cos_y * direction.x * step;
            camera.position_world_space[2] += -cos_y * direction.z * step;
            camera.position_world_space[2] +=  sin_y * direction.x * step;
            camera.position_world_space[1] +=  direction.y * step;
        }
    }

    //Pitch: Up/Down
    //Yaw: Left/Right
    void MovementController::rotateCamera(float pitchChange, float yawChange) {
        camera.rotation_x += pitchChange * ROTATION_RADIANS_PER_PIXEL;
        camera.rotation_y += yawChange * ROTATION_RADIANS_PER_PIXEL;

        if (camera.rotation_x > std::numbers::pi / 2) {
            camera.rotation_x = std::numbers::pi / 2;
        } else if (camera.rotation_x < -std::numbers::pi / 2) {
            camera.rotation_x = -std::numbers::pi / 2;
        }
    }
    }