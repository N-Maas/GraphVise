//
// Created by jannis on 1/15/26.
//

#include "Vertex.hpp"
#include "GraphSaver.hpp"

int Vertex::getVertexID() const {
    return vertexID;
}

int Vertex::getGroupID() const {
    return connectedGroupID;
}

void Vertex::setGroup(int groupID) {
    connectedGroupID = groupID;
}

const glm::vec3& Vertex::getCoordsVector() const {
    return coordsVector;
}

//Transparency überprüfen, ob Wert hat wenn ja überschreiben
//const ImVec4& Vertex::getVertexVec4() const {
//    return GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getGroupVec4();
//}

float Vertex::getTransparency() const {
    if (ownTransparency.has_value()) {
        return ownTransparency.value();
    }
    return GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getTransparency();
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