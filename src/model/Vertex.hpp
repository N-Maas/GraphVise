#ifndef THESIS_FRAMEWORK_VERTEX_HPP
#define THESIS_FRAMEWORK_VERTEX_HPP
#include <optional>
#include "imgui/imgui.h"


class Vertex {
    public:
        Vertex(const std::uint32_t  vertexID, const glm::vec3& coords)
            : vertexID(vertexID), connectedGroupID(0),
            coordsVector(coords) {}

        [[nodiscard]] std::uint32_t  getVertexID() const;
        [[nodiscard]] std::uint32_t  getGroupID() const;
        void setGroup(std::uint32_t  groupID);
        [[nodiscard]] const glm::vec3& getCoordsVector() const;
        [[nodiscard]] ImVec4 getVertexVec4() const;
        [[nodiscard]] float getTransparency() const;
        [[nodiscard]] bool setOwnTransparency(float transparency);
        void deleteOwnTransparency();
        void deleteVertex();

        Vertex& operator=(const Vertex& other) {
            if (this == &other) return *this;
            connectedGroupID = other.connectedGroupID;
            ownTransparency = other.ownTransparency;
            return *this;
        }

    private:
        const std::uint32_t  vertexID;
        std::uint32_t  connectedGroupID;
        std::optional<float> ownTransparency;
        const glm::vec3 coordsVector;
};


#endif