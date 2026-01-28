#include "Vertex.hpp"
#include <stdexcept>
#include "GraphSaver.hpp"


namespace graphvise {
    std::uint32_t  Vertex::getVertexID() const {
        return vertexID;
    }

    std::uint32_t  Vertex::getGroupID() const {
        return connectedGroupID;
    }

    void Vertex::setGroup(const std::uint32_t groupID) {
        connectedGroupID = groupID;
    }

    const glm::vec3& Vertex::getCoordsVector() const {
        return coordsVector;
    }

    ImVec4 Vertex::getVertexVec4() const {
        ImVec4 vertexVec = GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getGroupVec4();
        vertexVec.w = getTransparency();
        return vertexVec;
    }

    float Vertex::getTransparency() const {
        if (ownTransparency.has_value()) {
            return ownTransparency.value();
        }
        return GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getTransparency();
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