//
// Created by tim on 19.01.26.
//

#ifndef THESIS_FRAMEWORK_GROUPPARSER_HPP
#define THESIS_FRAMEWORK_GROUPPARSER_HPP
#include <string>
#include <expected>
#include "../Structs/GroupData.hpp"
#include "controller/Error.hpp"

namespace graphvise {
    class GroupParser {
    public:
        [[nodiscard]] std::expected<std::vector<GroupData>, Error> parseFile(const std::string& filePath);
    };
}

#endif