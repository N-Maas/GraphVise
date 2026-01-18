//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_EDGE_HPP
#define THESIS_FRAMEWORK_EDGE_HPP
#include <optional>
#include <vector>
#include "imgui/imgui.h"


class Edge {
    public:
        Edge(int edgeID, int firstVertexID, int secondVertexID)
            : edgeID(edgeID), connectedGroupID(0), firstVertexID(firstVertexID),
              secondVertexID(secondVertexID) {
        }

        [[nodiscard]] int getEdgeID() const;
        [[nodiscard]] int getGroupID() const;
        void setGroup(int groupID);
        [[nodiscard]] const std::vector<int>& getConnectingVerticesIDs() const;
        [[nodiscard]] const ImVec4& getEdgeVec4() const;
        [[nodiscard]] float getTransparency() const;
        [[nodiscard]] bool setOwnTransparency(float transparency);
        void deleteOwnTransparency();
        void deleteEdge();

        Edge& operator=(const Edge& other) {
            if (this == &other) return *this;
            connectedGroupID = other.connectedGroupID;
            ownTransparency = other.ownTransparency;
            return *this;
        }

    private:
        const int edgeID;
        int connectedGroupID;
        std::optional<float> ownTransparency;
        const int firstVertexID;
        const int secondVertexID;
};


#endif