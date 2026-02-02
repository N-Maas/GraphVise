#ifndef THESIS_FRAMEWORK_VERTEX_HPP
#define THESIS_FRAMEWORK_VERTEX_HPP
#include <optional>
#include "imgui/imgui.h"

namespace graphvise {
    class Vertex {
    public:
        explicit Vertex(const std::uint32_t  vertexID, const glm::vec3& coords)
            : vertexID(vertexID), connectedGroupID(0),
            coordsVector(coords) {}

        [[nodiscard]] std::uint32_t  getVertexID() const;
        [[nodiscard]] std::uint32_t  getGroupID() const;
        void setGroup(std::uint32_t  groupID);
        [[nodiscard]] glm::vec3 getCoordsVector() const;
        [[nodiscard]] ImVec4 getVertexVec4() const;
        void setOwnTransparency(float transparency);
        void deleteOwnTransparency();


    private:
        std::uint32_t  vertexID;
        std::uint32_t  connectedGroupID;
        std::optional<float> ownTransparency;
        glm::vec3 coordsVector;
    };
}

#endif