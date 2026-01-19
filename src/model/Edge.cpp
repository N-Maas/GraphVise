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

void Edge::setGroup(std::uint32_t new_groupID) {
    connectedGroupID = new_groupID;
}

std::vector<std::uint32_t > Edge::getConnectingVerticesIDs() const {
    std::vector verticesIDs = {firstVertexID, secondVertexID};
    return verticesIDs;
}

ImVec4 Edge::getEdgeVec4() const {
    ImVec4 edgeVec = GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getGroupVec4();
    edgeVec.w = getTransparency();
    return edgeVec;
}

float Edge::getTransparency() const {
    if (ownTransparency.has_value()) {
        return ownTransparency.value();
    }
    return GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getTransparency();
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