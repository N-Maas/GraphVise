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

std::optional<uint32_t> Graph::getEdgeIDByConnectingVerticesIDs(const std::uint32_t firstVertexID, const std::uint32_t secondVertexID) const {
    for (const Edge& edge : edges) {
        std::vector<std::uint32_t> connectingVerticesIDs = edge.getConnectingVerticesIDs();
        if (connectingVerticesIDs.at(0) == firstVertexID || connectingVerticesIDs.at(1) == firstVertexID) {
            if (connectingVerticesIDs.at(0) == secondVertexID || connectingVerticesIDs.at(1) == secondVertexID) {
                return edge.getEdgeID();
            }
        }
    }
    return std::nullopt;
}

 bool Graph::addVertex(const std::uint32_t vertexID, const glm::vec3& coords) {
    /*
    //todo throw out debug code when it works
    std::cout << "DEBUG: Graph::addVertex called with ID=" << vertexID
              << ", coords=(" << coords.x << "," << coords.y << "," << coords.z << ")" << std::endl;

    std::cout << "DEBUG: Before - vertices.size() = " << vertices.size()
              << ", vertices.capacity() = " << vertices.capacity() << std::endl;

    //todo uncomment when code is corrected

    std::cout << "DEBUG: Adding vertex ID=" << vertexID << std::endl;
    */

    //add to the end of vertex list
    vertices.emplace_back(vertexID, coords);

    return true; //ToDo Überprüfen, ob bereits ein Knoten mit der ID vorhanden ist, falls überhaubt nötig
    }



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