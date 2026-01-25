#ifndef THESIS_FRAMEWORK_GRAPHSAVER_HPP
#define THESIS_FRAMEWORK_GRAPHSAVER_HPP
#include "Graph.hpp"

using namespace model;

class GraphSaver {
    public:
        static GraphSaver& getGraphSaver();
        [[nodiscard]] Graph& getGraph();
        void setGraph(const Graph& new_graph);


    private:
        explicit GraphSaver();
        static GraphSaver instance;
        std::optional<Graph> graph;
};


#endif