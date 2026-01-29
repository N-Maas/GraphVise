//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP
#define THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP
#include <map>
#include <set>

#include "model/Graph.hpp"
#include "controller/Structs/GraphData.hpp"

namespace graphvise {
    class WEmbedController {
    public:
        Graph embedGraph(GraphData& graphData);

    private:
        std::map<int, std::set<int>> createNeighborhoodMap(GraphData& graphData);
    };
}
#endif //THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP