#include "GraphSaver.hpp"
#include <iostream>

#include "controller/Exporting/CachingController.hpp"


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

        if (graph.has_value())
        {
            CachingController::cacheCurrentGraph();
        }

        graph = new_graph;

        graph.value().initSortedVerticesAndEdges();
    }

}
