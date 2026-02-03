#include "Vertex.hpp"
#include <stdexcept>
#include "GraphSaver.hpp"

namespace graphvise {
    std::uint32_t  Vertex::getID() const {
        return vertexID;
    }

    std::uint32_t  Vertex::getGroupID() const {
        return connectedGroupID;
    }

    void Vertex::setGroup(const std::uint32_t groupID) {
        connectedGroupID = groupID;
        GraphSaver::getInstance().getGraph().updateSortedVertices();
    }

    glm::vec3 Vertex::getCoordsVector() const {
        return coordsVector;
    }

    ImVec4 Vertex::getVec4() const {
        ImVec4 vertexVec4 = GraphSaver::getInstance().getGraph().getGroupByID(connectedGroupID).getVec4();
        if (ownTransparency.has_value()) {
            vertexVec4.w = ownTransparency.value();
        }
        return vertexVec4;
    }

    void Vertex::setOwnTransparency(float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            ownTransparency = transparency;
            return;
        }
        throw std::out_of_range("Transparency must be between 0 and 1");
    }

    void Vertex::deleteOwnTransparency() {
        if (ownTransparency.has_value()) {
            ownTransparency.reset();
        }
    }
}