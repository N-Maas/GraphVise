//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_GRAPHPARSER_HPP
#define THESIS_FRAMEWORK_GRAPHPARSER_HPP

#include "../Structs/GraphData.hpp"
#include "controller/Error.hpp"

namespace graphvise {
    class GraphParser {
    public:
        //Destruktor
        virtual ~GraphParser();

        virtual std::expected<GraphData, Error> parseFile(std::filesystem::path filePath) = 0;
    };
}
#endif //THESIS_FRAMEWORK_GRAPHPARSER_HPP