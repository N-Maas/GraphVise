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

            if (!result.has_value()) {
                setError(result.error());
                return;
            }

            Graph embeddedGraph = wembedController.embedGraph(result.value());
            setParsedGraph(embeddedGraph);
            break;
        }

        case ParseFormat::GROUP: {
            std::expected<std::vector<GroupData>, Error> result = groupParser.parseFile(filePath);
            if (!result.has_value()) {
                std::cout << "Group threw error" << std::endl;
                setError(result.error());
                return;
            }
            setGroups(result.value());
            /*
            for (const GroupData& groupData : parsedGroupData) {
                //ToDo remove Debugging Stuff, if not needed anymore

                // std::cout << "----------Group declaration----------" << std::endl;
                // std::cout << "Groupname: "  << groupData.name << std::endl;
                // std::cout << "GroupColor: "  << groupData.color.x << " " << groupData.color.y << " " << groupData.color.z << " " << groupData.color.w << " " << std::endl;
                // std::cout << "VertexIDs: " << std::endl;
                // for (std::uint32_t vertexID : groupData.vertices) {
                //     std::cout << vertexID << " " << std::endl;
                // }
                // std::cout << "EdgeIDs: " << std::endl;
                // for (std::uint32_t edgeID : groupData.edges) {
                //     std::cout << edgeID << " " << std::endl;
                // }
                // std::cout << "ConnectingVertexIDs: " << std::endl;
                // for (std::uint32_t edgeID : groupData.edges) {
                //     std::cout << GraphSaver::getGraphSaver().getGraph().getEdgeByID(edgeID).getConnectingVerticesIDs().first << "   " << GraphSaver::getGraphSaver().getGraph().getEdgeByID(edgeID).getConnectingVerticesIDs().second << std::endl;
                // }
                GraphSaver::getGraphSaver().getGraph().addGroup(groupData.name, groupData.color ,groupData.vertices, groupData.edges);
            }*/
            break;
        }
        case ParseFormat::SUBGRAPH: {
            std::expected<GraphData, Error> result = txtParser.parseFile(std::move(filePath));

            if (!result.has_value()) {
                setError(result.error());
                std::cout << "Subgraph threw error" << std::endl;
                return;
            }

            std::optional<Error> verifyResult = verifySubgraph(result.value());
            if (!verifyResult.has_value()) {

                HighlightingData data;
                std::map<int, bool> usedVertexIDs;
                Graph& graph = GraphSaver::getInstance().getGraph();
                for (auto edge : result.value().edges) {
                    if (!usedVertexIDs.contains(edge.first)) {
                        data.vertices.emplace_back(edge.first);
                        usedVertexIDs[edge.first] = true;
                    }
                    if (!usedVertexIDs.contains(edge.second)) {
                        data.vertices.emplace_back(edge.second);
                        usedVertexIDs[edge.second] = true;
                    }
                    uint32_t edgeID = graph.getEdgeIDByConnectingVerticesIDs(edge.first, edge.second);
                    data.edges.emplace_back(edgeID);
                }

                setHighlightingSubgraph(data);
                std::cout << "valid subgraph" << std::endl;
            } else {
                setError(verifyResult.value());
                std::cout << "invalid subgraph" << static_cast<int>(verifyResult->getErrorType()) << std::endl;
            }

            break;
        }
        case ParseFormat::CNF: {
            break;
        }
    }
}

std::optional<Error> ParserController::verifySubgraph(GraphData graphData) {
    Graph& currentGraph = GraphSaver::getInstance().getGraph();

    if (graphData.vertexCount > currentGraph.getEdges().size()) {
        Error newError(ErrorType::TOO_MANY_VERTICES_IN_SUBGRAPH);
        return newError;
    }

    std::map<std::pair<int, int>, int> graphEdgeMap;
    for (auto edge : currentGraph.getEdges()) {
        std::pair<uint32_t, std::uint32_t> connectingVerticesIDs = edge.getConnectingVerticesIDs();
        std::pair<int, int> tempEdge(connectingVerticesIDs.first, connectingVerticesIDs.second);
        std::pair<int, int> tempEdgeInverted(connectingVerticesIDs.second, connectingVerticesIDs.first);
        graphEdgeMap[tempEdge] = 1;
        graphEdgeMap[tempEdgeInverted] = 1;
    }

    for (auto edge : graphData.edges) {
        if (!graphEdgeMap.contains(edge)) {
            std::cout << "invalid edge : " << edge.first << " " << edge.second << std::endl;
            Error newError(ErrorType::INVALID_SUBGRAPH_EDGE, std::to_string(edge.first) + " " + std::to_string(edge.second));
            return newError;
        }
    }

    return {};
}
}
