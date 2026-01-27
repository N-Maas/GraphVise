//
// Created by yannik on 14.01.26.
//

#include "TXTParser.hpp"

#include <fstream>
#include <expected>
#include <iostream>
#include <regex>
#include <vector>

#include "controller/Error.hpp"

#define HEADER_REGEX "\\s*v\\s*=\\s*(\\d+)\\s*,\\s*e\\s*=\\s*(\\d+)\\s*"
#define EDGE_REGEX "\\s*(\\d+)\\s+(\\d+)\\s*"
#define EMPTY_LINE_REGEX "\\s*"

[[nodiscard]] std::expected<GraphData, Error> TXTParser::parseFile(std::string filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        Error error(ErrorType::FILE_NOT_FOUND);
        return std::unexpected(error);
    }

    uint32_t currentLine = 1;

    std::regex headerRegex(HEADER_REGEX);
    std::smatch matches;
    std::string headerLine;
    std::getline(fileStream, headerLine);

    if (!std::regex_match(headerLine, matches, headerRegex)) {
        Error error(ErrorType::INVALID_FORMATTING);
        return std::unexpected(error);
    }

    //convert total vertex and edge count from string to uint32
    uint32_t vertexCount;
    try {
        vertexCount = std::stoi(matches[1].str());
    }
    catch (std::invalid_argument& exception) {
        Error error(ErrorType::INVALID_VERTEX_COUNT, headerLine, currentLine);
        return std::unexpected(error);
    }

    uint32_t edgeCount;
    try {
        edgeCount = std::stoi(matches[2].str());
    }
    catch (std::invalid_argument& exception) {
        Error error(ErrorType::INVALID_EDGE_COUNT, headerLine, currentLine);
        return std::unexpected(error);
    }

    std::regex edgeRegex(EDGE_REGEX);
    std::regex emptyLineRegex(EMPTY_LINE_REGEX);

    std::vector<std::pair<int, int>> edges;
    edges.reserve(edgeCount);

    for (std::string line; std::getline(fileStream, line);) {
        currentLine++;

        //skip empty lines
        if (std::regex_match(line, emptyLineRegex)) {
            continue;
        }

        //Check correct formatting
        if (!std::regex_match(line, matches, edgeRegex)) {
            Error error(ErrorType::INVALID_FORMATTING, line, currentLine);
        }

        //Convert vertex IDs to uint32
        uint32_t firstVertexID;
        try {
            firstVertexID = std::stoi(matches[1].str());
        } catch (std::invalid_argument& exception) {
            Error error(ErrorType::INVALID_FORMATTING, line, currentLine);
        }

        uint32_t secondVertexID;
        try {
            secondVertexID = std::stoi(matches[2].str());
        } catch (std::invalid_argument& exception) {
            Error error(ErrorType::INVALID_FORMATTING, line, currentLine);
        }

        //Return error if ids are too large
        if (firstVertexID > vertexCount - 1 || secondVertexID > vertexCount - 1) {
            Error error(ErrorType::NOT_A_VERTEX_ID, line, currentLine);
            return std::unexpected(error);
        }

        std::pair<int, int> edge(firstVertexID, secondVertexID);

        //Check for duplicate edges
        for (auto & currentEdge : edges) {
            if ((currentEdge.first == firstVertexID && currentEdge.second == secondVertexID) || (currentEdge.first == secondVertexID && currentEdge.second == firstVertexID)) {
                Error error(ErrorType::DUPLICATE_EDGE, line, currentLine);
                return std::unexpected(error);
            }
        }

        edges.push_back(edge);
    }

    //Check if all edges are there
    if (edges.size() != edgeCount) {
        Error error(ErrorType::INVALID_EDGE_COUNT);
        return std::unexpected(error);
    }

    GraphData graphData(vertexCount, edges);

    return graphData ;
}

GraphParser::~GraphParser() = default;
