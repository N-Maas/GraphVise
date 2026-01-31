#ifndef THESIS_FRAMEWORK_GRAPHSAVER_HPP
#define THESIS_FRAMEWORK_GRAPHSAVER_HPP
#include "Graph.hpp"

namespace graphvise {
    class GraphSaver {
    public:
        static GraphSaver& getGraphSaver();
        [[nodiscard]] Graph& getGraph();
        void setGraph(const Graph& new_graph);

        GraphSaver(const GraphSaver&) = delete;
        GraphSaver& operator=(const GraphSaver&) = delete;

    private:
        explicit GraphSaver();
        static GraphSaver instance;
        std::optional<Graph> graph;
    };
}

#endif