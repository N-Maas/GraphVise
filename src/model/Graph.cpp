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

Vertex& Graph::getVertexByID(const std::uint32_t  ID) {
    return vertices.at(ID);
}

Edge& Graph::getEdgeByID(const std::uint32_t ID) {
    return edges.at(ID);
}

Group& Graph::getGroupByID(const std::uint32_t ID) {
    return groups.at(ID);
}

CameraBookmark& Graph::getCameraBookmarkByID(const std::uint32_t ID) {
    return cameraBookmarks.at(ID);
}

// int Graph::getEdgeIDByConnectingVerticesIDs(int firstVertexID, int secondVertexID) const {
//
// }

// bool Graph::addVertex(int vertexID, const glm::vec3& coords) {
//
// }

void Graph::addEdge(uint32_t firstVertexID, uint32_t secondVertexID) {
    edges.emplace_back(edges.size(), firstVertexID, secondVertexID);
}

void Graph::addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<uint32_t>& verticesIDs, const std::vector<uint32_t>& edgesIDs) {
    groups.emplace_back(groups.size(), name, groupVec4);
    const std::size_t groupID = groups.size() - 1;
    for (const uint32_t ID : verticesIDs) {
        vertices.at(ID).setGroup(groupID);
    }
    for (const uint32_t ID : edgesIDs) {
        edges.at(ID).setGroup(groupID);
    }
}

void Graph::addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw) {
    cameraBookmarks.emplace_back(cameraBookmarks.size(), name, coords, pitch, yaw);
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

void Graph::deleteCameraBookmarks(const uint32_t cameraBookmarkID) {
    cameraBookmarks.erase(cameraBookmarks.begin() + cameraBookmarkID);
}

// void Graph::deleteGraph() {
//
// }