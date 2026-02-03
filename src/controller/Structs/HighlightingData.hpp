//
// Created by yannik on 02.02.26.
//

#ifndef THESIS_FRAMEWORK_HIGHLIGHTINGDATA_HPP
#define THESIS_FRAMEWORK_HIGHLIGHTINGDATA_HPP
#include <vector>

namespace graphvise {
    struct HighlightingData {
        std::vector<uint32_t> vertices;
        std::vector<uint32_t> edges;
    };
}

#endif //THESIS_FRAMEWORK_HIGHLIGHTINGDATA_HPP