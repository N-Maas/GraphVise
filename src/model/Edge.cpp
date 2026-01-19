//
// Created by jannis on 1/15/26.
//

#include "Edge.hpp"
#include "GraphSaver.hpp"

int Edge::getEdgeID() const {
    return edgeID;
}

int Edge::getGroupID() const {
    return connectedGroupID;
}

void Edge::setGroup(int new_groupID) {
    connectedGroupID = new_groupID;
}

const std::vector<int>& Edge::getConnectingVerticesIDs() const {
    std::vector verticesIDs = {firstVertexID, secondVertexID};
    return verticesIDs;
}

//Transparency überprüfen, ob Wert hat wenn ja überschreiben
//const ImVec4& Vertex::getVertexVec4() const {
//    return GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getGroupVec4();
//}

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