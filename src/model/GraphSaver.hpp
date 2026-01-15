//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_GRAPHSAVER_HPP
#define THESIS_FRAMEWORK_GRAPHSAVER_HPP
#include "Graph.hpp"


class GraphSaver {
    public:
        static GraphSaver& getGraphSaver();
        Graph& getGraph() const;
        void setGraph(const Graph& graph);

    private:
        Graph graph;
};


#endif //THESIS_FRAMEWORK_GRAPHSAVER_HPP