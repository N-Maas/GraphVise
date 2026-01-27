//
// Created by tim on 19.01.26.
//

#ifndef THESIS_FRAMEWORK_GROUPPARSER_HPP
#define THESIS_FRAMEWORK_GROUPPARSER_HPP
#include <string>

#include "../Structs/GroupData.hpp"
#include "../ThreadController.hpp"

namespace graphvise {
    class GroupParser {
    public:
        //Destruktor
        virtual ~GroupParser();

        virtual std::vector<GroupData> parseFile(std::string filePath) = 0;

    private:
        ThreadController threadController;
    };
}
#endif //THESIS_FRAMEWORK_GROUPPARSER_HPP