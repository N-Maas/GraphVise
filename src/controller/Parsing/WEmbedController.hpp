//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP
#define THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP
#include "model/Graph.hpp"
#include "controller/Structs/GraphData.hpp"

class WEmbedController {
    public:
        Graph embedGraph(GraphData& graphData);
};

#endif //THESIS_FRAMEWORK_WEMBEDCONTROLLER_HPP