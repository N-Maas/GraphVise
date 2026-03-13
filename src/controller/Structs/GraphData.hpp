//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_GRAPHDATA_HPP
#define THESIS_FRAMEWORK_GRAPHDATA_HPP
#include <vector>

namespace graphvise {
    struct GraphData {
        uint32_t vertexCount;
        std::vector<std::pair<uint32_t, uint32_t>> edges;
        std::string name;
    };
}

#endif //THESIS_FRAMEWORK_GRAPHDATA_HPP