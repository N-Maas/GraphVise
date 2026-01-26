//
// Created by jannis on 1/15/26.
//

#include "GraphSaver.hpp"

#include <iostream>

GraphSaver GraphSaver::instance;

GraphSaver::GraphSaver() {
}

GraphSaver& GraphSaver::getGraphSaver() {
    return instance;
}

std::optional<Graph>& GraphSaver::getGraph() {
    return graph;
}

void GraphSaver::setGraph(const Graph& new_graph) {
    graph = new_graph;
}