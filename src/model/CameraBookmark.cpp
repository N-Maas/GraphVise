#include "CameraBookmark.hpp"

namespace graphvise {
    const std::string& CameraBookmark::getName() const {
        return name;
    }

    void CameraBookmark::setName(const std::string& new_name) {
        name = new_name;
    }

    glm::vec3 CameraBookmark::getCoordsVector() const {
        return coordsVector;
    }

    float CameraBookmark::getPitch() const {
        return pitch;
    }

    float CameraBookmark::getYaw() const {
        return yaw;
    }
}