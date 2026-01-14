//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_TXTPARSER_HPP
#define THESIS_FRAMEWORK_TXTPARSER_HPP
#include "GraphParser.hpp"


class TXTParser : GraphParser{
    public:
        GraphData parseFile(std::string filePath) override;
};


#endif //THESIS_FRAMEWORK_TXTPARSER_HPP