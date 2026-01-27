//
// Created by tim on 19.01.26.
//

#ifndef THESIS_FRAMEWORK_GROUPPARSER_HPP
#define THESIS_FRAMEWORK_GROUPPARSER_HPP
#include <string>
#include "../ThreadController.hpp"

namespace graphvise {
    class GroupParser {
    public:
        [[nodiscard]] std::expected<GroupData, Error> parseFile(std::string filePath);
    };
}

#endif