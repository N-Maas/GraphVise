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
        Edge(const std::uint32_t  edgeID, const std::uint32_t  firstVertexID, const std::uint32_t  secondVertexID)
            : edgeID(edgeID), connectedGroupID(0), firstVertexID(firstVertexID),
              secondVertexID(secondVertexID) {
        }

        [[nodiscard]] std::uint32_t  getEdgeID() const;
        [[nodiscard]] std::uint32_t  getGroupID() const;
        void setGroup(std::uint32_t  groupID);
        [[nodiscard]] std::vector<std::uint32_t > getConnectingVerticesIDs() const;
        [[nodiscard]] ImVec4 getEdgeVec4() const;
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
        const std::uint32_t  edgeID;
        std::uint32_t  connectedGroupID;
        std::optional<float> ownTransparency;
        const std::uint32_t  firstVertexID;
        const std::uint32_t  secondVertexID;
};


#endif