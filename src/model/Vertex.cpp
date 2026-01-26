//
// Created by jannis on 1/15/26.
//

#include "Vertex.hpp"
#include "GraphSaver.hpp"

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
    /* todo remove
     *
    std::cout << "DEBUG: getCoordsVector() for vertex ID " << vertexID
              << ": (" << coordsVector.x << "," << coordsVector.y << "," << coordsVector.z << ")" << std::endl;
*/
    return coordsVector;
}

ImVec4 Vertex::getVertexVec4() const {
    auto& graph = GraphSaver::getGraphSaver().getGraph();
    if (!graph.has_value()) {//check if graph exists
        return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);//  red for testing todo remove this option when graph is no longer hardcoded, throw error
    }
    ImVec4 vertexVec = graph.value().getGroupByID(connectedGroupID).getGroupVec4();
    vertexVec.w = getTransparency();
    return vertexVec;
}

float Vertex::getTransparency() const {
    if (ownTransparency.has_value()) {
        return ownTransparency.value();
    }
    auto& graph = GraphSaver::getGraphSaver().getGraph();
    if (!graph.has_value()) {//check if graph exists
        return 1.0f;// todo remove this option when graph is no longer hardcoded, throw error
    }
    ImVec4 vertexVec = graph.value().getGroupByID(connectedGroupID).getGroupVec4();
    vertexVec.w = getTransparency();
    return vertexVec.w;
}

bool Vertex::setOwnTransparency(float transparency) {
    if (transparency >= 0.0f && transparency <= 1.0f) {
        ownTransparency = transparency;
        return true;
    }
    return false;
}

void Vertex::deleteOwnTransparency() {
    if (ownTransparency.has_value()) {
        ownTransparency.reset();
    }
}