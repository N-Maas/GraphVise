//
// Created by jannis on 1/15/26.
//

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
        Graph();
        [[nodiscard]] std::vector<Vertex>& getVertices() ;
        [[nodiscard]]std::vector<Edge>& getEdges();
        [[nodiscard]] std::vector<Group>& getGroups();
        [[nodiscard]] std::vector<CameraBookmark>& getCameraBookmarks();
        [[nodiscard]] Vertex& getVertexByID(int ID);
        [[nodiscard]] Edge& getEdgeByID(int ID);
        [[nodiscard]] Group& getGroupByID(int ID);
        [[nodiscard]] CameraBookmark& getCameraBookmarkByID(int ID);
        [[nodiscard]] int getEdgeIDByConnectingVerticesIDs(int firstVertexID, int secondVertexID) const;
        bool addVertex(int vertexID, const glm::vec3& coords);
        void addEdge(int firstVertexID, int secondVertexID);
        void addGroup(const std::string& name, const ImVec4& groupVec4, const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs);
        void addCameraBookmark(const std::string& name, const glm::vec3& coords, float pitch, float yaw);
        void highlightByID(const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs) const;
        void removeHighlightByID(const std::vector<int>& verticesIDs, const std::vector<int>& edgesIDs) const;
        void deleteAllGroups();
        void deleteCameraBookmarks(int cameraBookmarkID);
        void deleteGraph();

    private:
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::vector<Group> groups;
        std::vector<CameraBookmark> cameraBookmarks;
};


#endif