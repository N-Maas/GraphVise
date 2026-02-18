#include "Graph.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <glm/ext/matrix_transform.hpp>
#include "EdgeTransparencyCompare.hpp"
#include "VertexTransparencyCompare.hpp"

namespace graphvise {
    const std::vector<Vertex>& Graph::getVertices() const {
        return vertices;
    }

    const std::vector<Edge>& Graph::getEdges() const {
        return edges;
    }

    const std::vector<Group>& Graph::getGroups() const {
        return groups;
    }

    const std::vector<CameraBookmark>& Graph::getCameraBookmarks() const {
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
                    return edge.getID();
                }
            }
        }
        throw std::out_of_range("There is no edge between the specified nodes.");
    }

    void Graph::addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs) {
        groups.emplace_back(groups.size(), name, groupVec4);
        const std::size_t groupID = groups.size() - 1;
        if (!verticesIDs.empty()) {
            for (const std::uint32_t ID : verticesIDs) {
                vertices.at(ID).setGroup(groupID);
            }
            updateSortedVertices();
        }
        if (!edgesIDs.empty()) {
            for (const std::uint32_t ID : edgesIDs) {
                edges.at(ID).setGroup(groupID);
            }
            updateSortedEdges();
        }
    }

    void Graph::addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw) {
        cameraBookmarks.emplace_back(name, coords, pitch, yaw);
    }

    void Graph::highlightByID(const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs) {
        for (Vertex& vertex : vertices) {
            if (std::ranges::find(verticesIDs, vertex.getID()) == verticesIDs.end()) {
                vertex.setOwnTransparency(0.2f);
            } else {
                vertex.setOwnTransparency(1.0f);
            }
        }
        for (Edge& edge : edges) {
            if (std::ranges::find(edgesIDs, edge.getID()) == edgesIDs.end()) {
                edge.setOwnTransparency(0.2f);
            } else {
                edge.setOwnTransparency(1.0f);
            }
        }
        updateSortedVertices();
        updateSortedEdges();
    }

    void Graph::removeAllHighlights() {
        for (Vertex& vertex : vertices) {
            vertex.deleteOwnTransparency();
        }
        for (Edge& edge : edges) {
            edge.deleteOwnTransparency();
        }
        updateSortedVertices();
        updateSortedEdges();
    }

    void Graph::deleteAllGroups() {
        groups.clear();
        std::vector<std::uint32_t> verticesIDs;
        std::vector<std::uint32_t> edgesIDs;
        for (Vertex& vertex : vertices) {
            verticesIDs.push_back(vertex.getID());
        }
        for (Edge& edge : edges) {
            edgesIDs.push_back(edge.getID());
        }
        addGroup("Default-VertexGroup", ImVec4{255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f}, verticesIDs, std::vector<std::uint32_t>{});
        addGroup("Default-EdgeGroup", ImVec4{255 / 255.0f, 155 / 255.0f, 0 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, edgesIDs);
    }

    void Graph::deleteCameraBookmark(const std::uint32_t cameraBookmarkID) {
        if (cameraBookmarkID < cameraBookmarks.size()) {
            cameraBookmarks.erase(cameraBookmarks.begin() + cameraBookmarkID);
        } else {
            throw std::out_of_range("A camera bookmark with ID " + std::to_string(cameraBookmarkID) + " does not exist.");
        }
    }

    void Graph::setGroupTransparency(const std::uint32_t groupID, const float transparency){
        try {
            groups.at(groupID).setTransparency(transparency);
        } catch (std::out_of_range& e) {
            throw std::out_of_range("Group transparency is out of range [0,1]");
        }
        updateSortedVertices();
        updateSortedEdges();
    }

    ImVec4 Graph::getVertexVec4ByID(std::uint32_t vertexID) {
        ImVec4 vertexVec4 = getGroupByID(getVertexByID(vertexID).getConnectedGroupID()).getVec4();
        float ownTransparency = getVertexByID(vertexID).getOwnTransparency();
        if (ownTransparency >= 0) {
            vertexVec4.w = ownTransparency;
        }
        return vertexVec4;
    }

    ImVec4 Graph::getEdgeVec4ByID(std::uint32_t edgeID) {
        ImVec4 vertexVec4 = getGroupByID(getEdgeByID(edgeID).getConnectedGroupID()).getVec4();
        float ownTransparency = getEdgeByID(edgeID).getOwnTransparency();
        if (ownTransparency >= 0) {
            vertexVec4.w = ownTransparency;
        }
        return vertexVec4;
    }

    void Graph::updateSortedVertices(){
        std::ranges::sort(verticesSortedByTransparency, VertexTransparencyCompare{});
    }

    void Graph::updateSortedEdges() {
        std::ranges::sort(edgesSortedByTransparency, EdgeTransparencyCompare{});
    }

    std::vector<Vertex*> Graph::getVerticesSortedByTransparency() const{
        return verticesSortedByTransparency;
    }

    std::vector<Edge*> Graph::getEdgesSortedByTransparency() const{
        return edgesSortedByTransparency;
    }

    void Graph::initRenderingMatrixForEdge(Edge& edge) {
        std::uint32_t firstVertexID = edge.getConnectingVerticesIDs().first;
        std::uint32_t secondVertexID = edge.getConnectingVerticesIDs().second;
        glm::vec3 firstCoords = getVertexByID(firstVertexID).getCoordsVector();
        glm::vec3 secondCoords = getVertexByID(secondVertexID).getCoordsVector();

        glm::vec3 direction = secondCoords - firstCoords;
        edge.setLength(glm::length(direction));

        // Create model matrix
        glm::mat4 model = glm::mat4(1.0f);

        // Translate to midpoint
        glm::vec3 midpoint = (firstCoords + secondCoords) * 0.5f;
        model = glm::translate(model, midpoint);

        // Rotate to align with direction
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 axis = glm::cross(up, direction);
        float angle = acos(glm::dot(up, direction / edge.getLength()));
        edge.setMatrix(glm::rotate(model, angle, axis));
    }

    void Graph::initThisGraph() {
        for (Vertex& vertex : vertices) {
            verticesSortedByTransparency.emplace_back(&vertex);
        }
        for (Edge& edge : edges) {
            edgesSortedByTransparency.emplace_back(&edge);
            initRenderingMatrixForEdge(edge);
        }
    }
}