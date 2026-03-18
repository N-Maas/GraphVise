#include "Vertex.hpp"
#include "GraphSaver.hpp"

namespace graphvise {
    std::uint32_t Vertex::getID() const {
        return vertexID;
    }

    std::uint32_t Vertex::getConnectedGroupID() const {
        return connectedGroupID;
    }

    void Vertex::setGroup(const std::uint32_t groupID) {
        connectedGroupID = groupID;
    }

    glm::vec3 Vertex::getCoordsVector() const {
        return coordsVector;
    }

    void Vertex::setCoordsVector(glm::vec3 newCoordsVector) {
        this->coordsVector.x = newCoordsVector.x;
        this->coordsVector.y = newCoordsVector.y;
        this->coordsVector.z = newCoordsVector.z;
    }

    float Vertex::getOwnTransparency() const {
        return ownTransparency.value_or(-1.0f);
    }

    void Vertex::setOwnTransparency(float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            ownTransparency = transparency;
        }
    }

    void Vertex::deleteOwnTransparency() {
        if (ownTransparency.has_value()) {
            ownTransparency.reset();
        }
    }
}