#include "GraphSaver.hpp"
#include <iostream>

namespace graphvise {
    GraphSaver GraphSaver::instance;
    GraphSaver::GraphSaver() = default;

    GraphSaver& GraphSaver::getInstance() {
        return instance;
    }

    Graph& GraphSaver::getGraph() {
        if (!graph.has_value()) {
            throw std::runtime_error("No graph loaded");
        }
        return graph.value();
    }

    void GraphSaver::setGraph(const Graph& new_graph) {
        graph = new_graph;
        graph.value().initThisGraph();
    }
}