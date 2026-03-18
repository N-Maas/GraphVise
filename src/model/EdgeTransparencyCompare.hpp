#ifndef GRAPHVISE_EDGETRANSPARENCYCOMPARE_HPP
#define GRAPHVISE_EDGETRANSPARENCYCOMPARE_HPP
#include "Edge.hpp"
#include "GraphSaver.hpp"

struct EdgeTransparencyCompare {
    bool operator()(const graphvise::Edge* firstEdge, const graphvise::Edge* secondEdge) const {
        float firstTransparencyValue = graphvise::GraphSaver::getInstance().getGraph().getEdgeVec4ByID(firstEdge->getID()).w;
        float secondTransparencyValue = graphvise::GraphSaver::getInstance().getGraph().getEdgeVec4ByID(secondEdge->getID()).w;
        if (std::abs(firstTransparencyValue - secondTransparencyValue) > 1e-6f) {
            return firstTransparencyValue > secondTransparencyValue;
        }
        return firstEdge->getID() < secondEdge->getID();
    }
};
#endif