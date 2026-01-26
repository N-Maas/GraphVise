//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_GRAPHSAVER_HPP
#define THESIS_FRAMEWORK_GRAPHSAVER_HPP
#include "Graph.hpp"


class GraphSaver {
    public:
        GraphSaver();
        static GraphSaver& getGraphSaver();
        [[nodiscard]] std::optional<Graph> &getGraph();
        void setGraph(const Graph& new_graph);

    private:
        static GraphSaver instance;
        std::optional<Graph> graph;
};


#endif