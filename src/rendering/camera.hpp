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

#pragma once

#include <glm/glm.hpp>

/**
 * Holds state for a first person camera that characterizes the world to
 * projection space transform completely, except for the aspect ratio. It also
 * provides enough information to update the camera interactively. It does not
 * store any transforms or other redundant information. Such information has
 * to be computed as needed.
 */

enum CameraFocusMode {
    CENTER_OF_MASS,
    FREE
};

class Camera {
public:
    // The position of the camera in world space
    glm::vec3 position_world_space;
    // The rotation of the camera around the global y-axis in radians
    // The rotation of the camera around the local x-axis in radians. Without
    // rotation the camera looks into the negative z-direction.
    float rotation_y;
    float rotation_x;
    // The rotation that the camera would have if the cursor were moved to coordinate (0, 0) with rotate_camera enabled
    float rotation_y_0, rotation_x_0;
    // The distance of the near plane and the far plane to the camera position
    float near, far;
    // The vertical field of view (top to bottom) in radians
    float vertical_fov;
    // The default speed of this camera in meters per second when it moves along a single axis
    float speed;
    // 1 iff mouse movements are currently used to rotate the camera
    bool rotate_camera;

    Camera() :
        position_world_space(0, 0, 5),
        rotation_y(0),
        rotation_x(0),
        rotation_y_0(0),
        rotation_x_0(0),
        near(0.01f),
        far(1.0e3f),
        vertical_fov(0.33f*3.1415926536f),
        speed(2.0f),
        rotate_camera(false)
    {
    }

    // Constructs the world to view space transform for the given camera
    glm::mat4 get_world_to_view_space() const;

    // Constructs the view to projection space transform for the given camera and
    // the given width / height ratio
    glm::mat4 get_view_to_projection_space(float aspect_ratio) const;

    // Constructs the world to projection space transform for the given camera and
    // the given width / height ratio
    glm::mat4 get_world_to_projection_space(float aspect_ratio) const;
};
