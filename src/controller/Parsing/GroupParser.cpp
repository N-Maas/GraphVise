#include "GroupParser.hpp"
#include <expected>
#include <fstream>
#include <regex>
#include "model/GraphSaver.hpp"

#define HEADER_REGEX "\\s*Group\\s+([A-Za-z0-9_.-]+)\\s*(?:(\\d+)\\s*(?:(\\d+)\\s+(\\d+)\\s*(\\d+)?)?)?\\s*"
#define NODE_REGEX "(\\d+)"
#define EDGE_REGEX "\\s*(\\d+)\\s+(\\d+)\\s*"
#define EMPTY_LINE_REGEX "\\s*"

namespace graphvise {
    std::expected<std::vector<GroupData>, Error> GroupParser::parseFile(const std::string& filePath) {
        std::ifstream fileStream(filePath, std::ios::in);
        if (!fileStream.is_open()) {
            Error error(ErrorType::FILE_NOT_FOUND);
            return std::unexpected(error);
        }

        uint32_t currentLine = 0;
        std::vector<GroupData> allGroupData;
        std::regex headerRegex(HEADER_REGEX);
        std::regex nodeRegex(NODE_REGEX);
        std::regex edgeRegex(EDGE_REGEX);
        std::regex emptyLineRegex(EMPTY_LINE_REGEX);

        for (std::string line; std::getline(fileStream, line);) {
            currentLine++;
            std::smatch matches;

            if (std::regex_match(line, matches, headerRegex)) {
                //line defines a new Group
                GroupData currentGroupData = GroupData();
                currentGroupData.name = matches[1].str();
                int x = 153;        //Default Values, if non are specified in the Group-File
                int y = 61;
                int z = 0;
                int w = 100;        //Transparency from 0 to 100
                if (matches[2].matched && matches[3].matched) {
                    try {
                        x = std::stoi(matches[2].str());
                        y = std::stoi(matches[3].str());
                        z = std::stoi(matches[4].str());
                    } catch (const std::invalid_argument& exception) {
                        Error error(ErrorType::INVALID_COLOR_VALUE, line, currentLine);
                        return std::unexpected(error);
                    }
                    if (!(x >= 0 && y >= 0 && z >= 0 && x <= 255 && y <= 255 && z <= 255)) {
                        Error error(ErrorType::INVALID_COLOR_VALUE, line, currentLine);
                        return std::unexpected(error);
                    }
                    if (matches[5].matched) {
                        try {
                            w = std::stoi(matches[5].str());
                        } catch (const std::invalid_argument& exception) {
                            Error error(ErrorType::INVALID_TRANSPARENCY_VALUE, line, currentLine);
                            return std::unexpected(error);
                        }
                        if (!(w >= 0  && w <= 100)) {
                            Error error(ErrorType::INVALID_TRANSPARENCY_VALUE, line, currentLine);
                            return std::unexpected(error);
                        }
                    }
                } else if (matches[2].matched) {
                    try {
                        w = std::stoi(matches[2].str());
                    } catch (const std::invalid_argument& exception) {
                        Error error(ErrorType::INVALID_TRANSPARENCY_VALUE, line, currentLine);
                        return std::unexpected(error);
                    }
                    if (!(w >= 0  && w <= 100)) {
                        Error error(ErrorType::INVALID_TRANSPARENCY_VALUE, line, currentLine);
                        return std::unexpected(error);
                    }
                }
                currentGroupData.color.x = x / 255.0f;
                currentGroupData.color.y = y / 255.0f;
                currentGroupData.color.z = z / 255.0f;
                currentGroupData.color.w = w / 100.0f;
                allGroupData.push_back(currentGroupData);
            } else if (std::regex_match(line, matches, nodeRegex)) {
                //line defines a vertex for the last defined Group
                GroupData& currentGroupData = allGroupData.back();
                uint32_t vertexID;
                try {
                    vertexID = std::stoi(matches[1].str());
                } catch (const std::invalid_argument& exception) {
                    Error error(ErrorType::INVALID_VERTEX_ID, line, currentLine);
                    return std::unexpected(error);
                }
                if (vertexID > GraphSaver::getGraphSaver().getGraph().getVertices().size() - 1) {
                    Error error(ErrorType::INVALID_VERTEX_ID, line, currentLine);
                    return std::unexpected(error);
                }
                currentGroupData.vertices.push_back(vertexID);
            } else if (std::regex_match(line, matches, edgeRegex)) {
                //line defines an edge for the last defined Group
                GroupData& currentGroupData = allGroupData.back();
                uint32_t firstVertexID;
                uint32_t secondVertexID;
                try {
                    firstVertexID = std::stoi(matches[1].str());
                    secondVertexID = std::stoi(matches[2].str());
                } catch (const std::invalid_argument& exception) {
                    Error error(ErrorType::INVALID_EDGE_ID, line, currentLine);
                    return std::unexpected(error);
                }
                if (firstVertexID > GraphSaver::getGraphSaver().getGraph().getVertices().size() - 1 || secondVertexID > GraphSaver::getGraphSaver().getGraph().getVertices().size() - 1) {
                    Error error(ErrorType::INVALID_EDGE_ID, line, currentLine);
                    return std::unexpected(error);
                }
                try {
                    currentGroupData.edges.emplace_back(GraphSaver::getGraphSaver().getGraph().getEdgeIDByConnectingVerticesIDs(firstVertexID, secondVertexID));
                } catch (const std::out_of_range& exception) {
                    Error error(ErrorType::INVALID_EDGE_ID, line, currentLine);
                    return std::unexpected(error);
                }
            } else if (std::regex_match(line, matches, emptyLineRegex)) {
                //skips -> empty line
            } else {
                //line does not match any Regex -> Error
                Error error(ErrorType::INVALID_FORMATTING, line, currentLine);
                return std::unexpected(error);
            }
        }
        return allGroupData;
    }
}