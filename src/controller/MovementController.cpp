//
// Created by yannik on 14.01.26.
//

#include "MovementController.hpp"

#include <GLFW/glfw3.h>
#include "../rendering/enums.hpp"

#define SPRINT_INCREASE 10
#define ROTATION_RADIANS_PER_PIXEL 0.003f
#define MIN_VERTICAL_ANGLE 0.01
#define ZOOM_BASE_SPEED_MULTIPLIER 0.5
#define ZOOM_SPRINT_MULTIPLIER 10
#define MIN_DISTANCE 0.3

#define LIGHT_SOURCE_OFFSET glm::vec3(0, 0, 0)

namespace graphvise {

    //Using a 3D vector as direction
    //X Coordinate: Left/Right
    //Y Coordinate: Up/Down
    //Z Coordinate: Forwards/Backwards
    void MovementController::moveCamera(glm::vec3 direction, bool sprinting) {
        if (*camera.camera_focus_mode() == CameraFocusMode::FREE) {
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

        std::shared_ptr<Renderer> renderer = Renderer::getInstance();

        if (*renderer->light_source_movement_behaviour() == LightSourceMovementBehaviour::FOLLOW_CAMERA) {
            Renderer::getInstance()->setLightPos(camera.position_world_space + LIGHT_SOURCE_OFFSET);
        }
    }

    //Pitch: Up/Down
    //Yaw: Left/Right
    void MovementController::rotateCamera(float pitchChange, float yawChange) {
        if (*camera.camera_focus_mode() == CameraFocusMode::FREE) {
            camera.rotation_x += pitchChange * ROTATION_RADIANS_PER_PIXEL;
            camera.rotation_y += yawChange * ROTATION_RADIANS_PER_PIXEL;

            if (camera.rotation_x > std::numbers::pi / 2) {
                camera.rotation_x = std::numbers::pi / 2;
            } else if (camera.rotation_x < -std::numbers::pi / 2) {
                camera.rotation_x = -std::numbers::pi / 2;
            }
        } else if (*camera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS) {
            glm::vec3 camPos = camera.position_world_space - camera.focusPoint;

            double dist = glm::length(camPos);
            double verticalAngle = acos(camPos.y / dist);
            double horizontalAngle;

            if (camPos.x > 0) {
                horizontalAngle = atan(camPos.z / camPos.x);
            } else if (camPos.x == 0) {
                horizontalAngle = std::numbers::pi / 2;
                horizontalAngle *= camPos.z < 0 ? -1.0 : 1.0;
                horizontalAngle = camPos.z == 0 ? 0 : horizontalAngle;
            } else if (camPos.x < 0 && camPos.z >= 0) {
                horizontalAngle = atan(camPos.z / camPos.x) + std::numbers::pi;
            } else {
                horizontalAngle = atan(camPos.z / camPos.x) - std::numbers::pi;
            }

            horizontalAngle += yawChange * ROTATION_RADIANS_PER_PIXEL;
            verticalAngle += pitchChange * ROTATION_RADIANS_PER_PIXEL;

            if (verticalAngle < MIN_VERTICAL_ANGLE) {
                verticalAngle = MIN_VERTICAL_ANGLE;
            } else if (verticalAngle > std::numbers::pi - MIN_VERTICAL_ANGLE) {
                verticalAngle = std::numbers::pi - MIN_VERTICAL_ANGLE;
            }

            camPos.x = dist * sin(verticalAngle) * cos(horizontalAngle) + camera.focusPoint.x;
            camPos.y = dist * cos(verticalAngle) + camera.focusPoint.y;
            camPos.z = dist * sin(verticalAngle) * sin(horizontalAngle) + camera.focusPoint.z;

            camera.position_world_space = camPos;
            camera.lookAtFocus();
        }
    }

    void MovementController::zoom(float zoomValue, bool sprinting) {
        if (*camera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS) {
            glm::vec3 camPos = camera.position_world_space - camera.focusPoint;

            double dist = glm::length(camPos);
            double verticalAngle = acos(camPos.y / dist);
            double horizontalAngle;

            if (camPos.x > 0) {
                horizontalAngle = atan(camPos.z / camPos.x);
            } else if (camPos.x == 0) {
                horizontalAngle = std::numbers::pi / 2;
                horizontalAngle *= camPos.z < 0 ? -1.0 : 1.0;
                horizontalAngle = camPos.z == 0 ? 0 : horizontalAngle;
            } else if (camPos.x < 0 && camPos.z >= 0) {
                horizontalAngle = atan(camPos.z / camPos.x) + std::numbers::pi;
            } else {
                horizontalAngle = atan(camPos.z / camPos.x) - std::numbers::pi;
            }

            double sprintMultiplier = 1;
            if (sprinting) {
                sprintMultiplier = ZOOM_SPRINT_MULTIPLIER;
            }

            dist += zoomValue * ZOOM_BASE_SPEED_MULTIPLIER * sprintMultiplier;

            if (dist < MIN_DISTANCE) {
                dist = MIN_DISTANCE;
            }

            camPos.x = dist * sin(verticalAngle) * cos(horizontalAngle) + camera.focusPoint.x;
            camPos.y = dist * cos(verticalAngle) + camera.focusPoint.y;
            camPos.z = dist * sin(verticalAngle) * sin(horizontalAngle) + camera.focusPoint.z;

            camera.position_world_space = camPos;
        }
    }
}
