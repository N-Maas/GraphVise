//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_TXTPARSER_HPP
#define THESIS_FRAMEWORK_TXTPARSER_HPP
#include <expected>

#include "GraphParser.hpp"
#include "controller/Error.hpp"


class TXTParser : GraphParser {
    public:
        std::expected<GraphData, Error> parseFile(std::string filePath) override;
};


#endif //THESIS_FRAMEWORK_TXTPARSER_HPP