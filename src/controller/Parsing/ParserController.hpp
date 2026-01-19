//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#define THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#include <mutex>
#include <string>

#include "TXTParser.hpp"
#include "../Error.hpp"
#include "../Enums/ParseFormat.hpp"
#include "../Structs/GroupData.hpp"


class ParserController {
    public:
        void parseFile(std::pmr::string filePath, ParseFormat format);

    private:
        //TODO: Felder deklarieren, sobald die Klassen existieren
        TXTParser txtParser;

        std::mutex dataMutex;

        std::optional<Error> error;
        std::vector<GroupData> groups;
        GraphData highlightingSubgraph;
};


#endif //THESIS_FRAMEWORK_PARSERCONTROLLER_HPP