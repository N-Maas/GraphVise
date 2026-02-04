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

#include <iostream>
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

    void Camera::lookAtFocus() {
        //Horizontal Rotation
        double xDiff = position_world_space.x - focusPoint.x;
        double yDiff = position_world_space.y - focusPoint.y;
        double zDiff = position_world_space.z - focusPoint.z;
        if (zDiff > 0) {
            rotation_y = -atan(xDiff / zDiff);
        } else if (zDiff < 0) {
            rotation_y = std::numbers::pi - atan(xDiff / zDiff);
        } else if (xDiff >= 0) {
            rotation_y = -std::numbers::pi / 2;
        } else {
            rotation_y = std::numbers::pi / 2;
        }

        //Vertical Rotation
        double rotationDivisor = sqrt(xDiff * xDiff + zDiff * zDiff);
        rotation_x = -atan(yDiff / rotationDivisor);
    }
}