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
        std::vector<Vertex>& getVertices() const;
        std::vector<Edge>& getEdges() const;
        std::vector<Group>& getGroups() const;
        std::vector<CameraBookmark>& getCameraBookmarks() const;
        Vertex& getVertexByID(int ID) const;
        Edge& getEdgeByID(int ID) const;
        Group& getGroupByID(int ID) const;
        CameraBookmark& getCameraBookmarkByID(int ID) const;
        int getEdgeIDByConnectingVerticesIDs(int firstVertexID, int secondVertexID) const;
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


#endif //THESIS_FRAMEWORK_GRAPH_HPP