#include "camera.hpp"
#include <glm/gtx/transform.hpp>

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