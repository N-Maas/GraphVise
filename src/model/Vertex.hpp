#ifndef THESIS_FRAMEWORK_VERTEX_HPP
#define THESIS_FRAMEWORK_VERTEX_HPP
#include <optional>
#include <boost/optional/optional.hpp>

#include "imgui/imgui.h"


namespace graphvise
{
    class Vertex
    {
    public:
        explicit Vertex(const std::uint32_t vertexID, const glm::vec3& coords)
            : vertexID(vertexID), connectedGroupID(0),
              coordsVector(coords)
        {
        }
        Vertex(){}

        [[nodiscard]] std::uint32_t getID() const;
        [[nodiscard]] std::uint32_t getConnectedGroupID() const;
        void setGroup(std::uint32_t groupID);
        [[nodiscard]] glm::vec3 getCoordsVector() const;
        [[nodiscard]] float getOwnTransparency() const;
        void setOwnTransparency(float transparency);
        void deleteOwnTransparency();


        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & connectedGroupID;
            ar & coordsVector.x;
            ar & coordsVector.y;
            ar & coordsVector.z;
        }

    private:
        std::uint32_t vertexID;
        std::uint32_t connectedGroupID;
        boost::optional<float> ownTransparency;
        glm::vec3 coordsVector;
    };
}
#endif
