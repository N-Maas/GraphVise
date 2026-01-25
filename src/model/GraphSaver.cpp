#include "GraphSaver.hpp"
#include <iostream>

using namespace model;

GraphSaver GraphSaver::instance;

GraphSaver::GraphSaver() = default;

GraphSaver& GraphSaver::getGraphSaver() {
    return instance;
}

Graph& GraphSaver::getGraph() {
    if (!graph.has_value()) {
        throw std::logic_error("No graph has loaded yet");
    }
    return graph.value();
}

void GraphSaver::setGraph(const Graph& new_graph) {
    graph = new_graph;
}