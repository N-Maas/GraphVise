#ifndef THESIS_FRAMEWORK_GRAPH_HPP
#define THESIS_FRAMEWORK_GRAPH_HPP
#include <memory>
#include <string>
#include <vector>
#include "CameraBookmark.hpp"
#include "Edge.hpp"
#include "Group.hpp"
#include "Vertex.hpp"
#include "imgui/imgui.h"

namespace graphvise {
    struct VertexTransparencyCompare {
        bool operator()(const Vertex* firstVertex, const Vertex* secondVertex) const {
            float firstTransparencyValue = firstVertex->getVertexVec4().w;
            float secondTransparencyValue = secondVertex->getVertexVec4().w;
            if (std::abs(firstTransparencyValue - secondTransparencyValue) > 1e-6f) {
                return firstTransparencyValue > secondTransparencyValue;
            }
            return firstVertex->getVertexID() < secondVertex->getVertexID();
        }
    };

    struct EdgeTransparencyCompare {
        bool operator()(const Edge* firstEdge, const Edge* secondEdge) const {
            float firstTransparencyValue = firstEdge->getEdgeVec4().w;
            float secondTransparencyValue = secondEdge->getEdgeVec4().w;
            if (firstTransparencyValue != secondTransparencyValue) {
                return firstTransparencyValue > secondTransparencyValue;
            }
            return firstEdge->getEdgeID() < secondEdge->getEdgeID();
        }
    };

    class Graph {
    public:
        explicit Graph(const std::vector<glm::vec3>& verticesCoordinates, const std::vector<std::pair<std::uint32_t, std::uint32_t>>& edgesConnectedVerticesIDs) {
            addGroup("Default-VertexGroup", ImVec4{255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
            addGroup("Default-EdgeGroup", ImVec4{255 / 255.0f, 155 / 255.0f, 0 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
            vertices.reserve(verticesCoordinates.size());
            edges.reserve(edgesConnectedVerticesIDs.size());
            for (glm::vec3 coord : verticesCoordinates) {
                vertices.emplace_back(vertices.size(), coord);
            }
            for (std::pair edge : edgesConnectedVerticesIDs) {
                edges.emplace_back(edges.size(), edge.first, edge.second);
            }
        }

        [[nodiscard]] const std::vector<Vertex>& getVertices() const;
        [[nodiscard]] const std::vector<Edge>& getEdges() const;
        [[nodiscard]] const std::vector<Group>& getGroups() const;
        [[nodiscard]] const std::vector<CameraBookmark>& getCameraBookmarks() const;

        [[nodiscard]] Vertex& getVertexByID(std::uint32_t ID);
        [[nodiscard]] Edge& getEdgeByID(std::uint32_t ID);
        [[nodiscard]] Group& getGroupByID(std::uint32_t ID);
        [[nodiscard]] CameraBookmark& getCameraBookmarkByID(std::uint32_t ID);
        [[nodiscard]] std::uint32_t getEdgeIDByConnectingVerticesIDs(std::uint32_t firstVertexID, std::uint32_t secondVertexID) const;
        void addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs);
        void addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw);
        void highlightByID(const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs);
        void removeAllHighlights();
        void deleteAllGroups();
        void deleteCameraBookmarks(std::uint32_t  cameraBookmarkID);
        [[nodiscard]] std::vector<Vertex*> getVerticesSortedByTransparency() const;
        [[nodiscard]] std::vector<Edge *> getEdgesSortedByTransparency() const;
        void initSortedVerticesAndEdges();
        void updateSortedVertices();
        void updateSortedEdges();


    private:
        std::vector<Vertex> vertices;
        std::vector<Vertex*> verticesSortedByTransparency;
        std::vector<Edge> edges;
        std::vector<Edge*> edgesSortedByTransparency;
        std::vector<Group> groups;
        std::vector<CameraBookmark> cameraBookmarks;
    };
}
#endif