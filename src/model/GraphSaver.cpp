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

Graph& GraphSaver::getGraph() {
    //Throws exception, when no graph is loaded (program crashes)
    return graph.value();
}

void GraphSaver::setGraph(const Graph& new_graph) {
    graph = new_graph;
}