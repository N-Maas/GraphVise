//
// Created by tim on 19.01.26.
//

#include "CNFParser.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

#define HEADER_REGEX "p\\s+cnf\\s+(\\d+)\\s+(\\d+)\\s*"
#define EMPTY_LINE_REGEX "\\s*"
#define CLAUSE_REGEX "(-?[1-9]\\d*)(\\s-?[1-9]\\d*)*\\s0"
#define VARIABLE_DELIMITER ' '
#define NEGATION_SIGN '-'

namespace graphvise {
    std::expected<GraphData, Error> CNFParser::parseFile(std::filesystem::path filePath) {
        std::ifstream fileStream(filePath, std::ios::in);
        if (!fileStream.is_open()) {
            Error error(ErrorType::FILE_NOT_FOUND);
            return std::unexpected(error);
        }

        uint32_t currentLine = 1;

        //Check Header Line
        std::regex headerRegex(HEADER_REGEX);
        std::smatch matches;
        std::string headerLine;
        std::getline(fileStream, headerLine);

        if (!std::regex_match(headerLine, matches, headerRegex)) {
            Error error(ErrorType::INVALID_FORMATTING, headerLine, currentLine);
            return std::unexpected(error);
        }

        uint32_t variableCount;
        try {
            variableCount = std::stoi(matches[1].str());
        } catch ( std::invalid_argument& e) {
            Error error(ErrorType::INVALID_FORMATTING, headerLine, currentLine);
            return std::unexpected(error);
        }

        uint32_t clauseCount;
        try {
            clauseCount = std::stoi(matches[2].str());
        } catch ( std::invalid_argument& e) {
            Error error(ErrorType::INVALID_FORMATTING, headerLine, currentLine);
            return std::unexpected(error);
        }

        std::regex emptyLineRegex(EMPTY_LINE_REGEX);
        std::regex clauseRegex(CLAUSE_REGEX);

        std::map<uint32_t, std::set<uint32_t>> neighborhoods;

        uint32_t clausesParsed = 0;

        for (std::string line; std::getline(fileStream, line);) {
            currentLine++;

            //skip empty lines
            if (std::regex_match(line, emptyLineRegex)) {
                continue;
            }
            //Check correct formatting
            if (!std::regex_match(line, matches, clauseRegex)) {
                Error error(ErrorType::INVALID_FORMATTING, line, currentLine);
                return std::unexpected(error);
            }


            std::vector<uint32_t> currentVariables;
            std::stringstream lineStream(line);
            std::string variable;
            while (std::getline(lineStream, variable, VARIABLE_DELIMITER)) {
                if (variable[0] == NEGATION_SIGN) {
                    variable = variable.substr(1);
                }

                //Add all variables of current line to currentVariables
                uint32_t variableInt = std::stoi(variable);
                if (variableInt == 0) break;
                if (variableInt > variableCount) {
                    Error error(ErrorType::INVALID_VARIABLE_ID, line, currentLine);
                    return std::unexpected(error);
                }
                currentVariables.push_back(variableInt);
            }

            for (int i = 0; i < currentVariables.size(); i++) {
                std::set<uint32_t> neighborhood;
                //Create neighborhood for current variable
                for (int j = 0; j < currentVariables.size(); j++) {
                    if (i == j) continue;
                    neighborhood.insert(currentVariables.at(j));
                }
                //Check if variable already has some previously parsed neighbors
                if (!neighborhoods.contains(currentVariables.at(i))) {
                    neighborhoods[currentVariables.at(i)] = neighborhood;
                } else {
                    std::set<uint32_t>& oldNeighbors = neighborhoods[currentVariables.at(i)];
                    for (auto neighbor : neighborhood) {
                        oldNeighbors.insert(neighbor);
                    }
                }
            }
            clausesParsed++;
        }

        if (clausesParsed != clauseCount) {
            Error error(ErrorType::INVALID_CLAUSE_COUNT);
            return std::unexpected(error);
        }

        //Convert neighborhood to edge list
        std::vector<std::pair<uint32_t, uint32_t>> edges;
        std::map<std::pair<uint32_t, uint32_t>, bool> edgeMap;
        for (int i = 1; i < neighborhoods.size(); i++) {
            for (auto neighbor : neighborhoods[i]) {
                std::pair edge(i - 1, neighbor - 1);
                std::pair invertedEdge(neighbor - 1, i - 1);
                if (!edgeMap.contains(edge) && !edgeMap.contains(invertedEdge)) {
                    edges.push_back(edge);
                    edgeMap[edge] = true;
                    edgeMap[invertedEdge] = true;
                }
            }
        }
        fileStream.close();

        auto name = filePath.stem().string();

        GraphData graphData(variableCount, edges, name);
        return graphData;
    }
}
