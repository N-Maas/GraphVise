//
// Created by yannik on 14.01.26.
//

#include "ParserController.hpp"

#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "model/GraphSaver.hpp"

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
            std::expected<GraphData, Error> result = txtParser.parseFile(std::move(filePath));
            GraphData parsedGraphData;
            if (result.has_value()) {
                parsedGraphData = std::move(result.value());
            } else {
                error = result.error();
            }

            std::optional<Error> verifyResult = verifySubgraph(parsedGraphData);
            if (!verifyResult.has_value()) {
                highlightingSubgraph = parsedGraphData;
                std::cout << "valid" << std::endl;
            } else {
                error = verifyResult.value();
            }

            if (error.has_value())
                throw std::exception();

            break;
        }
        case ParseFormat::CNF: {
            break;
        }
    }
}

std::optional<Error> ParserController::verifySubgraph(GraphData graphData) {
    Graph& currentGraph = GraphSaver::getGraphSaver().getGraph();

    if (graphData.vertexCount > currentGraph.getEdges().size()) {
        Error newError(ErrorType::TOO_MANY_VERTICES_IN_SUBGRAPH);
        return newError;
    }

    std::map<std::pair<int, int>, int> graphEdgeMap;
    for (auto edge : currentGraph.getEdges()) {
        std::vector<uint32_t> connectingVerticesIDs = edge.getConnectingVerticesIDs();
        std::pair<int, int> tempEdge(connectingVerticesIDs[0], connectingVerticesIDs[1]);
        graphEdgeMap[tempEdge] = 1;
    }

    for (auto edge : graphData.edges) {
        if (!graphEdgeMap.contains(edge)) {
            Error newError(ErrorType::INVALID_SUBGRAPH_EDGE, std::to_string(edge.first) + " " + std::to_string(edge.second));
            return newError;
        }
    }

    return {};
}

std::optional<Graph> ParserController::getGraph() {
        return parsedGraph;
    }
}
