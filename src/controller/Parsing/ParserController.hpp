//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#define THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#include <mutex>
#include <string>
#include "GroupParser.hpp"
#include "TXTParser.hpp"
#include "WEmbedController.hpp"
#include "../Error.hpp"
#include "../Enums/ParseFormat.hpp"
#include "../Structs/GroupData.hpp"
#include "model/Graph.hpp"

namespace graphvise {
    class ParserController {
    public:
        void parseFile(std::string filePath, ParseFormat format);
        std::optional<Graph> getGraph();
        std::optional<GraphData>& getHighlightingSubgraph() {
            return highlightingSubgraph;
        }

    private:
        std::optional<Error> verifySubgraph(GraphData graphData);

        //TODO: Felder deklarieren, sobald die Klassen existieren
        TXTParser txtParser;
        GroupParser groupParser;
        WEmbedController wembedController;

        std::mutex dataMutex;

        std::optional<Error> error;
        std::optional<Graph> parsedGraph;
        std::optional<std::vector<GroupData>> groups;
        std::optional<GraphData> highlightingSubgraph;
    };
}

#endif //THESIS_FRAMEWORK_PARSERCONTROLLER_HPP