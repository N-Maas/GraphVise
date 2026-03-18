#ifndef THESIS_FRAMEWORK_GRAPH_HPP
#define THESIS_FRAMEWORK_GRAPH_HPP
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "CameraBookmark.hpp"
#include "Edge.hpp"
#include "Group.hpp"
#include "Vertex.hpp"
#include "imgui/imgui.h"

namespace graphvise {
    class Graph {
    public:
        explicit Graph(const std::vector<glm::vec3>& verticesCoordinates, const std::vector<std::pair<std::uint32_t, std::uint32_t>>& edgesConnectedVerticesIDs, const std::string& name) : name(name) {
            addGroup("Default-VertexGroup", ImVec4{255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
            addGroup("Default-EdgeGroup", ImVec4{255 / 255.0f, 155 / 255.0f, 0 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
            vertices.reserve(verticesCoordinates.size());
            edges.reserve(edgesConnectedVerticesIDs.size());
            float maxDistance = 0;
            for (glm::vec3 coord : verticesCoordinates) {
                vertices.emplace_back(vertices.size(), coord);
                float currentDistance = std::sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
                if (currentDistance > maxDistance) {
                    maxDistance = currentDistance;
                }
            }
            radius = maxDistance;
            for (std::pair edge : edgesConnectedVerticesIDs) {
                edges.emplace_back(edges.size(), edge.first, edge.second);
            }
        }
        [[nodiscard]] float getRadius() const;
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
        void deleteCameraBookmark(std::uint32_t  cameraBookmarkID);
        void setGroupTransparency(std::uint32_t groupID, float transparency);
        void scaleGraph(float newScale);
        [[nodiscard]] ImVec4 getVertexVec4ByID(std::uint32_t vertexID);
        [[nodiscard]] ImVec4 getEdgeVec4ByID(std::uint32_t edgeID);

        void updateSortedVertices();

        void updateSortedEdges();

        [[nodiscard]] const std::vector<Vertex *> &getVerticesSortedByTransparency() const;
        [[nodiscard]] const std::vector<Edge *> &getEdgesSortedByTransparency() const;
        void initThisGraph();

        [[nodiscard]] const std::string& getName() const { return name; }

            template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & name;
            ar & vertices;
            ar & edges;
            ar & groups;
            ar & cameraBookmarks;
            ar & radius;
        }

    private:
        void initRenderingMatrixForEdge(Edge& edge);
        float manualScale = 1;
        float radius = 1;
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<Vertex*> verticesSortedByTransparency;
        std::vector<Edge> edges;
        std::vector<Edge*> edgesSortedByTransparency;
        std::vector<Group> groups;
        std::vector<CameraBookmark> cameraBookmarks;
    };
}
#endif