//
// Created by yannik on 05.02.26.
//

#ifndef GRAPHVISE_EXPORTDATA_HPP
#define GRAPHVISE_EXPORTDATA_HPP
#include <vector>

namespace graphvise {
    struct PNGExportData {
        std::vector<unsigned char> pixels;
        int screenWidth;
        int screenHeight;
    };
}

#endif //GRAPHVISE_EXPORTDATA_HPP