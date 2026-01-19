//
// Created by jannis on 1/15/26.
//

#include "GraphSaver.hpp"

GraphSaver& GraphSaver::getGraphSaver() {
    static GraphSaver graphSaver;
    return graphSaver;
}

Graph& GraphSaver::getGraph() {
    return graph;
}

void GraphSaver::setGraph(const Graph& new_graph) {
    graph = new_graph;
}