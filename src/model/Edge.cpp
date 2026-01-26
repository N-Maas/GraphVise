//
// Created by jannis on 1/15/26.
//

#include "Edge.hpp"
#include "GraphSaver.hpp"

std::uint32_t  Edge::getEdgeID() const {
    return edgeID;
}

std::uint32_t  Edge::getGroupID() const {
    return connectedGroupID;
}

void Edge::setGroup(const std::uint32_t new_groupID) {
    connectedGroupID = new_groupID;
}

std::vector<std::uint32_t> Edge::getConnectingVerticesIDs() const {
    std::vector verticesIDs = {firstVertexID, secondVertexID};
    return verticesIDs;
}

ImVec4 Edge::getEdgeVec4() const {
    auto& graph = GraphSaver::getGraphSaver().getGraph();
    if (!graph.has_value()) {//check if graph exists
        return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green for testing todo remove when graph isnt hardcoded
    }
    ImVec4 edgeVec = graph.value().getGroupByID(connectedGroupID).getGroupVec4();
    edgeVec.w = getTransparency();
    return edgeVec;
}

float Edge::getTransparency() const {
    if (ownTransparency.has_value()) {
        return ownTransparency.value();
    }
    auto& graph = GraphSaver::getGraphSaver().getGraph();
    if (!graph.has_value()) {//check if graph exists
        return 1.0f;  // todo remove when graph isnt hardcoded
    }
    ImVec4 edgeVec = graph.value().getGroupByID(connectedGroupID).getGroupVec4();
    edgeVec.w = getTransparency();
    return edgeVec.w;
}

bool Edge::setOwnTransparency(float transparency) {
    if (transparency >= 0.0f && transparency <= 1.0f) {
        ownTransparency = transparency;
        return true;
    }
    return false;
}

void Edge::deleteOwnTransparency() {
    if (ownTransparency.has_value()) {
        ownTransparency.reset();
    }
}