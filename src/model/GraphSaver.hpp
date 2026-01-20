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
        std::optional<Graph> graph;
};


#endif