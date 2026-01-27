//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_GROUPDATA_HPP
#define THESIS_FRAMEWORK_GROUPDATA_HPP
#include <string>
#include <vector>

#include "imgui/imgui.h"

namespace graphvise {
    struct GroupData {
        std::string name;
        ImVec4 color;
        std::vector<int> vertices;
        std::vector<int> edges;
    };
}

#endif //THESIS_FRAMEWORK_GROUPDATA_HPP