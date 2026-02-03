//  Copyright (C) 2021, Christoph Peters, Karlsruhe Institute of Technology
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
//
// https://github.com/MomentsInGraphics/vulkan_renderer/


#include "./camera.hpp"
#include <glm/gtx/transform.hpp>

namespace graphvise {
    glm::mat4 Camera::get_world_to_view_space() const
    {
        glm::mat4 rotY = glm::rotate(rotation_y, glm::vec3(0, 1, 0));
        glm::mat4 rotX = glm::rotate(rotation_x, glm::vec3(-1, 0, 0));
        glm::mat4 translate = glm::translate(-position_world_space);
        return rotX *  rotY * translate;
    }

    glm::mat4 Camera::get_view_to_projection_space(float aspect_ratio) const
    {
        return glm::perspective(vertical_fov, aspect_ratio, this->near, this->far);
    }

    glm::mat4 Camera::get_world_to_projection_space(float aspect_ratio) const
    {
        return get_view_to_projection_space(aspect_ratio) * get_world_to_view_space();
    }

    void Camera::lookAt(const glm::vec3& target) {
        // Calculate direction from camera to target
        glm::vec3 direction = target - position_world_space;

        // Calculate horizontal distance
        float horizontalDistance = sqrt(direction.x * direction.x + direction.z * direction.z);

        // Calculate y rotation (around Y-axis)
        // atan2 gives angle from x-axis, but we need angle from z-axis
        // Camera's default forward is negative z, so we adjust
        rotation_y = atan2f(-direction.x, -direction.z);

        // Calculate x rotation (around X-axis)
        // atan2(y, horizontalDistance) gives vertical angle
        rotation_x = atan2f(direction.y, horizontalDistance);
    }

    void Camera::orbitAround(const glm::vec3& target, float distance) {
        // First, make the camera look at the target
        lookAt(target);

        // Then move the camera to orbit distance along the view direction
        // View direction is (sin(rotation_y), tan(rotation_x), -cos(rotation_y))
        glm::vec3 viewDirection(
            sinf(rotation_y) * cosf(rotation_x),
            sinf(rotation_x),
            -cosf(rotation_y) * cosf(rotation_x)
        );

        // Position camera at target minus distance along view direction
        // (since view direction points from camera to target)
        position_world_space = target - viewDirection * distance;
    }
}