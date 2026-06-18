//
// Created by yannik on 14.01.26.
//

#include "MovementController.hpp"

#include <GLFW/glfw3.h>

#include <cmath>
#include "../rendering/Enums.hpp"
#include "model/GraphSaver.hpp"

#define SPRINT_INCREASE 5
#define ROTATION_RADIANS_PER_PIXEL 0.003f
#define MIN_VERTICAL_ANGLE 0.01f
#define ZOOM_BASE_SPEED_MULTIPLIER 0.5f
#define ZOOM_SPRINT_MULTIPLIER 6
#define MIN_DISTANCE 0.3f
#define DEFAULT_SPEED 2.0f

namespace graphvise
{
    //Using a 3D vector as direction
    //X Coordinate: Left/Right
    //Y Coordinate: Up/Down
    //Z Coordinate: Forwards/Backwards
    void MovementController::moveCamera(glm::vec3 direction, bool sprinting)
    {
        if (*camera.camera_focus_mode() == CameraFocusMode::FREE)
        {
            static double last_time = 0.0;
            const double now = glfwGetTime();
            const double elapsed_time = (last_time == 0.0) ? 0.0 : (now - last_time);
            const auto time_delta = static_cast<float>(elapsed_time);
            last_time = now;
            float step = time_delta * DEFAULT_SPEED;
            float graphRadius = GraphSaver::getInstance().getGraph().getRadius();
            step *= sprinting ? SPRINT_INCREASE * std::log(graphRadius) : 1.0f;

            // std::cout << step << " " << graphRadius << std::endl;

            const float cos_y = cosf(camera.rotation_y), sin_y = sinf(camera.rotation_y);
            const float cos_x = cosf(camera.rotation_x), sin_x = sinf(camera.rotation_x);

            // Since the camera can't rotate along the z axis, left/right movement is always
            // along the x-z plane and up/down movement is always along the y-z plane.
            // However, forward/backward movement can point in any direction -> spherical coordinates tranformation

            // left / right
            camera.position_world_space[0] += cos_y * direction.x * step;
            camera.position_world_space[2] += sin_y * direction.x * step;

            // up / down
            camera.position_world_space[1] += cos_x * direction.y * step;
            camera.position_world_space[2] += sin_x * direction.y * step;

            // forward / backward
            camera.position_world_space[0] += cos_x * sin_y * direction.z * step;
            camera.position_world_space[1] += sin_x * direction.z * step;
            camera.position_world_space[2] += -cos_x * cos_y * direction.z * step;

        } else if (*camera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS)
        {
            
            constexpr uint32_t scale = 7;
            rotateCamera(-(scale * direction.y), -(scale * direction.x));
            zoom((-direction.z / 2 ), sprinting);
            
        }


        updateLightSourcePosition();
    }


    //Pitch: Up/Down
    //Yaw: Left/Right
    void MovementController::rotateCamera(float pitchChange, float yawChange)
    {
        if (*camera.camera_focus_mode() == CameraFocusMode::FREE)
        {
            {
                camera.rotation_x += pitchChange * ROTATION_RADIANS_PER_PIXEL;
                camera.rotation_y += yawChange * ROTATION_RADIANS_PER_PIXEL;

                if (camera.rotation_x > std::numbers::pi / 2)
                {
                    camera.rotation_x = std::numbers::pi / 2;
                }
                else if (camera.rotation_x < -std::numbers::pi / 2)
                {
                    camera.rotation_x = -std::numbers::pi / 2;
                }
            }
        }
        else if (*camera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS)
        {
            glm::vec3 camPos = camera.position_world_space - camera.focusPoint;

            double dist = glm::length(camPos);
            double verticalAngle = acos(camPos.y / dist);
            double horizontalAngle;

            if (camPos.x > 0)
            {
                horizontalAngle = atan(camPos.z / camPos.x);
            }
            else if (camPos.x == 0)
            {
                horizontalAngle = std::numbers::pi / 2;
                horizontalAngle *= camPos.z < 0 ? -1.0 : 1.0;
                horizontalAngle = camPos.z == 0 ? 0 : horizontalAngle;
            }
            else if (camPos.x < 0 && camPos.z >= 0)
            {
                horizontalAngle = atan(camPos.z / camPos.x) + std::numbers::pi;
            }
            else
            {
                horizontalAngle = atan(camPos.z / camPos.x) - std::numbers::pi;
            }

            horizontalAngle += yawChange * ROTATION_RADIANS_PER_PIXEL;
            verticalAngle += pitchChange * ROTATION_RADIANS_PER_PIXEL;

            if (verticalAngle < MIN_VERTICAL_ANGLE)
            {
                verticalAngle = MIN_VERTICAL_ANGLE;
            }
            else if (verticalAngle > std::numbers::pi - MIN_VERTICAL_ANGLE)
            {
                verticalAngle = std::numbers::pi - MIN_VERTICAL_ANGLE;
            }

            camPos.x = dist * sin(verticalAngle) * cos(horizontalAngle) + camera.focusPoint.x;
            camPos.y = dist * cos(verticalAngle) + camera.focusPoint.y;
            camPos.z = dist * sin(verticalAngle) * sin(horizontalAngle) + camera.focusPoint.z;

            camera.position_world_space = camPos;
            camera.lookAtFocus();
            updateLightSourcePosition();
        }
    }

    void MovementController::zoom(float zoomValue, bool sprinting)
    {
        if (*camera.camera_focus_mode() == CameraFocusMode::CENTER_OF_MASS) {
            glm::vec3 camPos = camera.position_world_space - camera.focusPoint;

            double dist = glm::length(camPos);
            double verticalAngle = acos(camPos.y / dist);
            double horizontalAngle;

            if (camPos.x > 0)
            {
                horizontalAngle = atan(camPos.z / camPos.x);
            }
            else if (camPos.x == 0)
            {
                horizontalAngle = std::numbers::pi / 2;
                horizontalAngle *= camPos.z < 0 ? -1.0 : 1.0;
                horizontalAngle = camPos.z == 0 ? 0 : horizontalAngle;
            }
            else if (camPos.x < 0 && camPos.z >= 0)
            {
                horizontalAngle = atan(camPos.z / camPos.x) + std::numbers::pi;
            }
            else
            {
                horizontalAngle = atan(camPos.z / camPos.x) - std::numbers::pi;
            }

            double sprintMultiplier = 1;
            if (sprinting)
            {
                if (dist > 1)
                {
                    sprintMultiplier = ZOOM_SPRINT_MULTIPLIER;
                }
            }

            float distanceMultiplier = dist > 1.5 ? log(dist) : 0.5;

            dist += zoomValue * ZOOM_BASE_SPEED_MULTIPLIER * sprintMultiplier * distanceMultiplier;

            if (dist < MIN_DISTANCE)
            {
                dist = MIN_DISTANCE;
            }

            camPos.x = dist * sin(verticalAngle) * cos(horizontalAngle) + camera.focusPoint.x;
            camPos.y = dist * cos(verticalAngle) + camera.focusPoint.y;
            camPos.z = dist * sin(verticalAngle) * sin(horizontalAngle) + camera.focusPoint.z;

            camera.position_world_space = camPos;
            updateLightSourcePosition();
        } else if (*camera.camera_focus_mode() == CameraFocusMode::FREE) {


            moveCamera(glm::vec3(0, 0, zoomValue * 500), sprinting);

            
        }
    }

    void MovementController::updateLightSourcePosition()
    {
        if (*Renderer::getInstance()->light_source_movement_behaviour() == LightSourceMovementBehaviour::FOLLOW_CAMERA)
        {
            Renderer::getInstance()->setLightPos(camera.position_world_space);
        }
    }
}
