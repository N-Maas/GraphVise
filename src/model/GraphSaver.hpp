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
        [[nodiscard]] Graph& getGraph();
        void setGraph(const Graph& new_graph);

    private:
        static GraphSaver instance;
        //TODO: is optional ok here?
        std::optional<Graph> graph;
};


#endif