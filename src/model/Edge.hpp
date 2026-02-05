#ifndef THESIS_FRAMEWORK_EDGE_HPP
#define THESIS_FRAMEWORK_EDGE_HPP
#include <optional>
#include <vector>
#include "imgui/imgui.h"

namespace graphvise {
    class Edge {
    public:
        explicit Edge(const std::uint32_t  edgeID, const std::uint32_t  firstVertexID, const std::uint32_t  secondVertexID)
            : edgeID(edgeID), connectedGroupID(1), connectingVerticesIDS(firstVertexID, secondVertexID)  {
        }

        [[nodiscard]] std::uint32_t getID() const;
        [[nodiscard]] std::uint32_t getConnectedGroupID() const;
        void setGroup(std::uint32_t groupID);
        [[nodiscard]] std::pair<std::uint32_t, std::uint32_t> getConnectingVerticesIDs() const;
        [[nodiscard]] ImVec4 getVec4() const;
        void setOwnTransparency(float transparency);
        void deleteOwnTransparency();


    private:
        std::uint32_t  edgeID;
        std::uint32_t  connectedGroupID;
        std::optional<float> ownTransparency;
        std::pair<std::uint32_t, std::uint32_t> connectingVerticesIDS;
    };
}

#endif