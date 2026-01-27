//
// Created by yannik on 14.01.26.
//

#include "ParserController.hpp"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace graphvise {
void ParserController::parseFile(std::string filePath, ParseFormat format) {
    switch (format) {
        default:
            throw std::invalid_argument("Provided ParseFormat has not yet been defined");
        case ParseFormat::TXT: {
            std::expected<GraphData, Error> result = txtParser.parseFile(std::move(filePath));
            GraphData parsedGraphData;
            if (result.has_value()) {
                parsedGraphData = std::move(result.value());
            } else {
                error = result.error();
                return;
            }
            parsedGraph = wembedController.embedGraph(parsedGraphData);
            break;
        }
        case ParseFormat::GROUP: {
            break;
        }
        case ParseFormat::SUBGRAPH: {
            break;
        }
        case ParseFormat::CNF: {
            break;
        }
    }
}

    std::optional<Graph> ParserController::getGraph() {
        return parsedGraph;
    }
}