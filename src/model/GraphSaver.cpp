#include "GraphSaver.hpp"
#include <iostream>

namespace graphvise {
    GraphSaver GraphSaver::instance;

    GraphSaver::GraphSaver() = default;

    GraphSaver& GraphSaver::getGraphSaver() {
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
    }

    //helper methods to get graph
    Group& GraphSaver::getGroupForEdge(const Edge& edge) {
        if (!graph.has_value()) {
            throw std::runtime_error("No graph loaded");
        }
        return graph.value().getGroupByID(edge.getGroupID());
    }

    Group& GraphSaver::getGroupByID(uint32_t groupID) {
        if (!graph.has_value()) {
            throw std::runtime_error("No graph loaded");
        }
        return graph.value().getGroupByID(groupID);
    }


    std::vector<Vertex>& GraphSaver::getVertices() {
        if (!graph.has_value()) {
            throw std::runtime_error("No graph loaded");
        }
        return graph.value().getVertices();
    }

}