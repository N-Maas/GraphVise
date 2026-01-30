#include "Graph.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace graphvise {
    const std::vector<Vertex>& Graph::getVertices() {
        return vertices;
    }

    const std::vector<Edge>& Graph::getEdges() {
        return edges;
    }

    const std::vector<Group>& Graph::getGroups() {
        return groups;
    }

    const std::vector<CameraBookmark>& Graph::getCameraBookmarks() {
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

    std::uint32_t Graph::getEdgeIDByConnectingVerticesIDs(const std::uint32_t firstVertexID, const std::uint32_t secondVertexID) const {
        if (firstVertexID == secondVertexID) {
            throw std::out_of_range("An edge requires two different nodes.");
        }
        for (const Edge& edge : edges) {
            std::pair<std::uint32_t, std::uint32_t> connectingVerticesIDs = edge.getConnectingVerticesIDs();
            if (connectingVerticesIDs.first == firstVertexID || connectingVerticesIDs.second == firstVertexID) {
                if (connectingVerticesIDs.first == secondVertexID || connectingVerticesIDs.second == secondVertexID) {
                    return edge.getEdgeID();
                }
            }
        }
        throw std::out_of_range("There is no edge between the specified nodes.");
    }

    void Graph::addEdge(std::uint32_t firstVertexID, std::uint32_t secondVertexID) {
        edges.emplace_back(edges.size(), firstVertexID, secondVertexID);
    }

    void Graph::addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs) {
        groups.emplace_back(groups.size(), name, groupVec4);
        const std::size_t groupID = groups.size() - 1;
        for (const std::uint32_t ID : verticesIDs) {
            vertices.at(ID).setGroup(groupID);
        }
        for (const std::uint32_t ID : edgesIDs) {
            edges.at(ID).setGroup(groupID);
        }
    }

    void Graph::addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw) {
        cameraBookmarks.emplace_back(name, coords, pitch, yaw);
    }

    void Graph::highlightByID(const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs) {
        for (Vertex& vertex : vertices) {
            if (std::ranges::find(verticesIDs, vertex.getVertexID()) == verticesIDs.end()) {
                vertex.setOwnTransparency(0.2f);
            } else {
                vertex.setOwnTransparency(1.0f);
            }
        }
        for (Edge& edge : edges) {
            if (std::ranges::find(edgesIDs, edge.getEdgeID()) == edgesIDs.end()) {
                edge.setOwnTransparency(0.2f);
            } else {
                edge.setOwnTransparency(1.0f);
            }
        }
    }

    void Graph::removeAllHighlights() {
        for (Vertex& vertex : vertices) {
            vertex.deleteOwnTransparency();
        }
        for (Edge& edge : edges) {
            edge.deleteOwnTransparency();
        }
    }

    void Graph::deleteAllGroups() {
        groups.clear();
        addGroup("Default-Group", ImVec4{51 / 255.0f, 0.0f, 34 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
        for (Vertex& vertex : vertices) {
            vertex.setGroup(0);
        }
        for (Edge& edge : edges) {
            edge.setGroup(0);
        }
    }

    void Graph::deleteCameraBookmarks(const std::uint32_t cameraBookmarkID) {
        if (cameraBookmarkID < cameraBookmarks.size()) {
            cameraBookmarks.erase(cameraBookmarks.begin() + cameraBookmarkID);
        } else {
            throw std::out_of_range("A camera bookmark with ID " + std::to_string(cameraBookmarkID) + " does not exist.");
        }
    }
}