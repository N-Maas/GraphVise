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
        Edge(int edgeID, int firstVertexID, int secondVertexID);
        int getEdgeID() const;
        int getGroupID() const;
        void setGroup(int groupID);
        const std::vector<int>& getConnectingVerticesIDs() const;
        const ImVec4& getEdgeVec4() const;
        float getTransparency() const;
        bool setOwnTransparency(float transparency);
        void deleteOwnTransparency();
        void deleteEdge();

    private:
        const int edgeID;
        int connectedGroupID;
        std::optional<float> ownTransparency;
        const int firstVertexID;
        const int secondVertexID;
};


#endif //THESIS_FRAMEWORK_EDGE_HPP