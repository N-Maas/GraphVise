//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_GRAPHPARSER_HPP
#define THESIS_FRAMEWORK_GRAPHPARSER_HPP
#include <string>

#include "../Structs/GraphData.hpp"
#include "controller/Error.hpp"

#endif //THESIS_FRAMEWORK_GRAPHPARSER_HPP

class GraphParser {
    public:
    //Destruktor
    virtual ~GraphParser();

    virtual std::expected<GraphData, Error> parseFile(std::string filePath) = 0;
};