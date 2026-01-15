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
        int getVertexID() const;
        int getGroupID() const;
        void setGroup(int groupID);
        const glm::vec3& getCoordsVector() const;
        const ImVec4& getVertexVec4() const;
        float getTransparency() const;
        bool setOwnTransparency(float transparency);
        void deleteOwnTransparency();
        void deleteVertex();

    private:
        const int vertexID;
        int connectedGroupID;
        std::optional<float> ownTransparency;
        const glm::vec3 coordsVector;
};


#endif //THESIS_FRAMEWORK_VERTEX_HPP