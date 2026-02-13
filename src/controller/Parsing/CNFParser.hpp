//
// Created by tim on 19.01.26. 
//

#ifndef THESIS_FRAMEWORK_CNFPARSER_HPP
#define THESIS_FRAMEWORK_CNFPARSER_HPP
#include <expected>
#include <filesystem>
#include <string>

#include "GraphParser.hpp"
#include "../Structs/GraphData.hpp"

namespace graphvise {
    class CNFParser : GraphParser {
    public:
        //Destruktor
        virtual ~CNFParser() = default;

        std::expected<GraphData, Error> parseFile(std::filesystem::path filePath) override;
    };
}

#endif //THESIS_FRAMEWORK_CNFPARSER_HPP