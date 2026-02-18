#ifndef GRAPHVISE_VERTEXTRANSPARENCYCOMPARE_HPP
#define GRAPHVISE_VERTEXTRANSPARENCYCOMPARE_HPP
#include "GraphSaver.hpp"
#include "Vertex.hpp"

struct VertexTransparencyCompare {
    bool operator()(const graphvise::Vertex* firstVertex, const graphvise::Vertex* secondVertex) const {
        float firstTransparencyValue = graphvise::GraphSaver::getInstance().getGraph().getVertexVec4ByID(firstVertex->getID()).w;
        float secondTransparencyValue = graphvise::GraphSaver::getInstance().getGraph().getVertexVec4ByID(secondVertex->getID()).w;
        if (std::abs(firstTransparencyValue - secondTransparencyValue) > 1e-6f) {
            return firstTransparencyValue > secondTransparencyValue;
        }
        return firstVertex->getID() < secondVertex->getID();
    }
};

#endif