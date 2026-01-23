//
// Created by yannik on 14.01.26.
//

#include "ParserController.hpp"

#include <stdexcept>
#include <utility>

void ParserController::parseFile(std::string filePath, ParseFormat format) {
    switch (format) {
        default:
            throw std::invalid_argument("Provided ParseFormat has not yet been defined");
        case ParseFormat::TXT:
            std::expected<GraphData, Error> result = txtParser.parseFile(std::move(filePath));
            GraphData parsedGraphData;
            if (result.has_value()) {
                parsedGraphData = std::move(result.value());
            } else {
                error = result.error();
                return;
            }
            parsedGraph = wembedController.embedGraph(std::move(parsedGraphData));
            break;
    }
}
