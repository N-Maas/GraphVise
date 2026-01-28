//
// Created by tim on 19.01.26. 
//

#ifndef THESIS_FRAMEWORK_CNFPARSER_HPP
#define THESIS_FRAMEWORK_CNFPARSER_HPP

#include "../Structs/GraphData.hpp"
#include "model/GraphSaver.hpp"

namespace graphvise {
    class SubgraphParser {
    public:
        SubgraphParser() = default;

        //Destruktor
        virtual ~SubgraphParser() = default;

        virtual GraphData parseFile(std::string filePath) = 0;
    };
}
#endif //THESIS_FRAMEWORK_CNFPARSER_HPP
