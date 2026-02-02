#include "Edge.hpp"
#include <stdexcept>
#include "GraphSaver.hpp"

namespace graphvise {
    std::uint32_t  Edge::getEdgeID() const {
        return edgeID;
    }

    std::uint32_t  Edge::getGroupID() const {
        return connectedGroupID;
    }

    void Edge::setGroup(const std::uint32_t groupID) {
        connectedGroupID = groupID;
        GraphSaver::getGraphSaver().getGraph().updateSortedEdges();
    }

    std::pair<std::uint32_t, std::uint32_t> Edge::getConnectingVerticesIDs() const {
        return connectingVerticesIDS;
    }

    ImVec4 Edge::getEdgeVec4() const {
        ImVec4 edgeVec4 = GraphSaver::getGraphSaver().getGraph().getGroupByID(connectedGroupID).getGroupVec4();
        if (ownTransparency.has_value()) {
            edgeVec4.w = ownTransparency.value();
        }
        return edgeVec4;
    }

    void Edge::setOwnTransparency(float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            ownTransparency = transparency;
            return;
        }
        throw std::out_of_range("Transparency must be between 0 and 1");
    }

    void Edge::deleteOwnTransparency() {
        if (ownTransparency.has_value()) {
            ownTransparency.reset();
        }
    }
}