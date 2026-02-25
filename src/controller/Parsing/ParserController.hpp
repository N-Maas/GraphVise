//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#define THESIS_FRAMEWORK_PARSERCONTROLLER_HPP
#include <filesystem>
#include <mutex>
#include <string>
#include <utility>

#include "CNFParser.hpp"
#include "GroupParser.hpp"
#include "TXTParser.hpp"
#include "WEmbedController.hpp"
#include "../Error.hpp"
#include "../Enums/ParseFormat.hpp"
#include "../Structs/GroupData.hpp"
#include "controller/Structs/HighlightingData.hpp"
#include "model/Graph.hpp"

namespace graphvise {
    class ParserController {
    public:
        void parseFile(std::filesystem::path filePath, ParseFormat format);

        std::optional<Graph> getParsedGraph() {
            std::lock_guard lock(dataMutex);
            return parsedGraph;
        }

        std::optional<HighlightingData> getHighlightingSubgraph() {
            std::lock_guard lock(dataMutex);
            return highlightingSubgraph;
        }

        std::optional<std::vector<GroupData>> getGroups() {
            std::lock_guard lock(dataMutex);
            return groups;
        }

        std::optional<Error> getError() {
            std::lock_guard lock(dataMutex);
            return error;
        }

        void resetData() {
            std::lock_guard lock(dataMutex);
            parsedGraph.reset();
            highlightingSubgraph.reset();
            groups.reset();
            error.reset();
        }
    private:
        std::optional<Error> verifySubgraph(GraphData graphData);

        TXTParser txtParser;
        CNFParser cnfParser;
        GroupParser groupParser;
        WEmbedController wembedController;

        std::mutex dataMutex;

        std::optional<Error> error;
        std::optional<Graph> parsedGraph;
        std::optional<std::vector<GroupData>> groups;
        std::optional<HighlightingData> highlightingSubgraph;

        void setHighlightingSubgraph(HighlightingData highlightingSubgraph) {
            std::lock_guard lock(dataMutex);
            this->highlightingSubgraph = std::move(highlightingSubgraph);
        }

        void setParsedGraph(Graph parsedGraph) {
            std::lock_guard lock(dataMutex);
            this->parsedGraph = std::move(parsedGraph);
        }

        void setGroups(std::vector<GroupData> groups) {
            std::lock_guard lock(dataMutex);
            this->groups = std::move(groups);
        }

        void setError(Error error) {
            std::lock_guard lock(dataMutex);
            this->error = std::move(error);
        }
    };
}

#endif //THESIS_FRAMEWORK_PARSERCONTROLLER_HPP