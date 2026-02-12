#ifndef THESIS_FRAMEWORK_EDGE_HPP
#define THESIS_FRAMEWORK_EDGE_HPP

#include <boost/optional/optional.hpp>

#include "imgui/imgui.h"

namespace graphvise {
    class Edge {
    public:
        explicit Edge(const std::uint32_t edgeID, const std::uint32_t firstVertexID, const std::uint32_t secondVertexID)
            : edgeID(edgeID), connectedGroupID(1), connectingVerticesIDs(firstVertexID, secondVertexID)  {
        }
        Edge(){}

        [[nodiscard]] std::uint32_t getID() const;
        [[nodiscard]] std::uint32_t getConnectedGroupID() const;
        void setGroup(std::uint32_t groupID);
        [[nodiscard]] std::pair<std::uint32_t, std::uint32_t> getConnectingVerticesIDs() const;
        [[nodiscard]] ImVec4 getVec4() const;
        void setOwnTransparency(float transparency);
        void deleteOwnTransparency();


        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & edgeID;
            ar & connectedGroupID;
            ar & ownTransparency;
            ar & connectingVerticesIDs.first;
            ar & connectingVerticesIDs.second;


        }

    private:
        std::uint32_t edgeID;
        std::uint32_t connectedGroupID;
        boost::optional<float> ownTransparency;
        std::pair<std::uint32_t, std::uint32_t> connectingVerticesIDs;
    };
}
#endif