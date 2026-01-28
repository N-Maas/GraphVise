//
// Created by yannik on 14.01.26.
//

#include "ParserController.hpp"
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
            std::expected<std::vector<GroupData>, Error> result = groupParser.parseFile(filePath);
            std::vector<GroupData> parsedGroupData;
            if (result.has_value()) {
                parsedGroupData = std::move(result.value());
            } else {
                error = result.error();
                return;
            }
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
                //     std::cout << GraphSaver::getGraphSaver().getGraph().getEdgeByID(edgeID).getConnectingVerticesIDs().at(0) << "   " << GraphSaver::getGraphSaver().getGraph().getEdgeByID(edgeID).getConnectingVerticesIDs().at(1) << std::endl;
                // }
                GraphSaver::getGraphSaver().getGraph().addGroup(groupData.name, groupData.color ,groupData.vertices, groupData.edges);
            }
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