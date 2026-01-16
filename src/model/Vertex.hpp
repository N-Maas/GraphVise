//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_VERTEX_HPP
#define THESIS_FRAMEWORK_VERTEX_HPP
#include <optional>
#include "imgui/imgui.h"


class Vertex {
    public:
        Vertex(int vertexID, const glm::vec3& coords);
        [[nodiscard]] int getVertexID() const;
        [[nodiscard]] int getGroupID() const;
        void setGroup(int groupID);
        [[nodiscard]] const glm::vec3& getCoordsVector() const;
        [[nodiscard]] const ImVec4& getVertexVec4() const;
        [[nodiscard]] float getTransparency() const;
        [[nodiscard]] bool setOwnTransparency(float transparency);
        void deleteOwnTransparency();
        void deleteVertex();

    private:
        const int vertexID;
        int connectedGroupID;
        std::optional<float> ownTransparency;
        const glm::vec3 coordsVector;
};


#endif //THESIS_FRAMEWORK_VERTEX_HPP