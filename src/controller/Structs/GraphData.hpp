//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_GRAPHDATA_HPP
#define THESIS_FRAMEWORK_GRAPHDATA_HPP
#include <cstdint>
#include <vector>

#endif //THESIS_FRAMEWORK_GRAPHDATA_HPP

struct GraphData {
    uint32_t vertexCount;
    std::vector<std::vector<uint32_t>> edges;
};