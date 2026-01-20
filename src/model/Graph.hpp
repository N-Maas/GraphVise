#ifndef THESIS_FRAMEWORK_GRAPH_HPP
#define THESIS_FRAMEWORK_GRAPH_HPP
#include <string>
#include <vector>
#include "CameraBookmark.hpp"
#include "Edge.hpp"
#include "Group.hpp"
#include "Vertex.hpp"
#include "imgui/imgui.h"


class Graph {
    public:
        explicit Graph(const std::uint32_t quantityOfVertices) {
            vertices.reserve(quantityOfVertices);
        }

        [[nodiscard]] std::vector<Vertex>& getVertices() ;
        [[nodiscard]]std::vector<Edge>& getEdges();
        [[nodiscard]] std::vector<Group>& getGroups();
        [[nodiscard]] std::vector<CameraBookmark>& getCameraBookmarks();
        [[nodiscard]] Vertex& getVertexByID(std::uint32_t  ID);
        [[nodiscard]] Edge& getEdgeByID(std::uint32_t  ID);
        [[nodiscard]] Group& getGroupByID(std::uint32_t  ID);
        [[nodiscard]] CameraBookmark& getCameraBookmarkByID(std::uint32_t  ID);
        [[nodiscard]] std::optional<uint32_t> getEdgeIDByConnectingVerticesIDs(std::uint32_t  firstVertexID, std::uint32_t  secondVertexID) const;
        bool addVertex(std::uint32_t  vertexID, const glm::vec3& coords);
        void addEdge(std::uint32_t  firstVertexID, std::uint32_t  secondVertexID);
        void addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<std::uint32_t >& verticesIDs, const std::vector<std::uint32_t >& edgesIDs);
        void addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw);
        void highlightByID(const std::vector<std::uint32_t >& verticesIDs, const std::vector<std::uint32_t >& edgesIDs) const;
        void removeHighlightByID(const std::vector<std::uint32_t >& verticesIDs, const std::vector<std::uint32_t >& edgesIDs) const;
        void deleteAllGroups();
        void deleteCameraBookmarks(std::uint32_t  cameraBookmarkID);

    private:
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::vector<Group> groups;
        std::vector<CameraBookmark> cameraBookmarks;
};


#endif