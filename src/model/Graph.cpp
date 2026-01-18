//
// Created by jannis on 1/15/26.
//

#include "Graph.hpp"

std::vector<Vertex>& Graph::getVertices() {
    return vertices;
}

std::vector<Edge>& Graph::getEdges() {
    return edges;
}

std::vector<Group>& Graph::getGroups() {
    return groups;
}

std::vector<CameraBookmark>& Graph::getCameraBookmarks() {
    return cameraBookmarks;
}

Vertex& Graph::getVertexByID(int ID) {
    return vertices.at(ID);
}

Edge& Graph::getEdgeByID(int ID) {
    return edges.at(ID);
}

Group& Graph::getGroupByID(int ID) {
    return groups.at(ID);
}

CameraBookmark& Graph::getCameraBookmarkByID(int ID) {
    return cameraBookmarks.at(ID);
}

// int Graph::getEdgeIDByConnectingVerticesIDs(int firstVertexID, int secondVertexID) const {
//
// }

// bool Graph::addVertex(int vertexID, const glm::vec3& coords) {
//
// }

void Graph::addEdge(int firstVertexID, int secondVertexID) {
    edges.push_back(Edge(edges.size(), firstVertexID, secondVertexID));
}

void Graph::addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs) {
    groups.push_back(Group(groups.size(), name, groupVec4));
    for (int ID : verticesIDs) {
        vertices.at(ID).setGroup(groups.size() - 1);
    }
    for (int ID : edgesIDs) {
        edges.at(ID).setGroup(groups.size() - 1);
    }
}

void Graph::addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw) {
    cameraBookmarks.push_back(CameraBookmark(cameraBookmarks.size(), name, coords, pitch, yaw));
}

// void Graph::highlightByID(const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs) const {
//
// }

// void Graph::removeHighlightByID(const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs) const {
//
// }

void Graph::deleteAllGroups() {
    groups.clear();
}

void Graph::deleteCameraBookmarks(int cameraBookmarkID) {
    cameraBookmarks.erase(cameraBookmarks.begin() + cameraBookmarkID);
}

// void Graph::deleteGraph() {
//
// }