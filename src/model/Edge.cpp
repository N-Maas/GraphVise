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

    void Edge::setGroup(const std::uint32_t new_groupID) {
        connectedGroupID = new_groupID;
    }

    std::pair<std::uint32_t, std::uint32_t> Edge::getConnectingVerticesIDs() const {
        std::pair verticesIDs = {firstVertexID, secondVertexID};
        return verticesIDs;
    }

    ImVec4 Edge::getEdgeVec4() const {
        return GraphSaver::getGraphSaver().getGroupForEdge(*this).getGroupVec4();
    }

    float Edge::getTransparency() const {
        auto& graphSaver = GraphSaver::getGraphSaver();
        auto graph = graphSaver.getGraph();
        return graph.getGroupByID(connectedGroupID).getTransparency();
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