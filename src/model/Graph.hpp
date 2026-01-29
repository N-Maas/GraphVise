#ifndef THESIS_FRAMEWORK_GRAPH_HPP
#define THESIS_FRAMEWORK_GRAPH_HPP
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
        explicit Graph(const std::vector<glm::vec3>& coordinates) {
            for (glm::vec3 coord : coordinates) {
                vertices.emplace_back(vertices.size(), coord);
            }
            addGroup("Default-Group", ImVec4{51 / 255.0f, 0.0f, 34 / 255.0f, 1.0f}, std::vector<std::uint32_t>{}, std::vector<std::uint32_t>{});
        }

        // todo rausschmeissen wenn der Graph eingelesen und gerendered werden kann
        Graph() = default;
        /*
        // todo rausschmeissen wenn der Graph eingelesen und gerendered werden kann
        explicit Graph(int vertexNumber) {
            vertices.reserve(vertexNumber);

        }
        */
        // todo rausschmeissen wenn der Graph eingelesen und gerendered werden kann
        void addVertex(std::uint32_t vertexID, const glm::vec3& coords);

        [[nodiscard]] std::vector<Vertex>& getVertices() ;
        [[nodiscard]] std::vector<Edge>& getEdges();
        [[nodiscard]] std::vector<Group>& getGroups();
        [[nodiscard]] std::vector<CameraBookmark>& getCameraBookmarks();
        [[nodiscard]] Vertex& getVertexByID(std::uint32_t  ID);
        [[nodiscard]] Edge& getEdgeByID(std::uint32_t  ID);
        [[nodiscard]] Group& getGroupByID(std::uint32_t  ID);
        [[nodiscard]] CameraBookmark& getCameraBookmarkByID(std::uint32_t  ID);
        [[nodiscard]] std::uint32_t getEdgeIDByConnectingVerticesIDs(std::uint32_t  firstVertexID, std::uint32_t  secondVertexID) const;
        void addEdge(std::uint32_t  firstVertexID, std::uint32_t  secondVertexID);
        void addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs);
        void addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw);
        void highlightByID(const std::vector<std::uint32_t>& verticesIDs, const std::vector<std::uint32_t>& edgesIDs);
        void removeAllHighlights();
        void deleteAllGroups();
        void deleteCameraBookmarks(std::uint32_t  cameraBookmarkID);


    private:
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::vector<Group> groups;
        std::vector<CameraBookmark> cameraBookmarks;
    };
}
#endif