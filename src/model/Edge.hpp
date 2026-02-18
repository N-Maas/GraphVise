#ifndef THESIS_FRAMEWORK_EDGE_HPP
#define THESIS_FRAMEWORK_EDGE_HPP
#include <optional>
#include "imgui/imgui.h"

namespace graphvise {
    class Edge {
    public:
        explicit Edge(const std::uint32_t edgeID, const std::uint32_t firstVertexID, const std::uint32_t secondVertexID)
            : edgeID(edgeID), connectedGroupID(1), connectingVerticesIDs(firstVertexID, secondVertexID)  {
        }

        [[nodiscard]] std::uint32_t getID() const;
        [[nodiscard]] std::uint32_t getConnectedGroupID() const;
        void setGroup(std::uint32_t groupID);
        [[nodiscard]] std::pair<std::uint32_t, std::uint32_t> getConnectingVerticesIDs() const;
        [[nodiscard]] float getOwnTransparency() const;
        void setOwnTransparency(float transparency);
        void deleteOwnTransparency();

        void setLength(float length);
        float getLength() const;
        void setMatrix(const glm::mat4& matrix);
        const glm::mat4& getMatrix() const;


    private:
        std::uint32_t edgeID;
        std::uint32_t connectedGroupID;
        std::optional<float> ownTransparency;
        std::pair<std::uint32_t, std::uint32_t> connectingVerticesIDs;
        float length;
        glm::mat4 matrix;
    };
}
#endif