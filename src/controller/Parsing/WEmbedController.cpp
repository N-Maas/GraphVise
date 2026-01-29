//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"

#include "AdamOptimizer.hpp"
#include "wembed.h"

namespace graphvise {
    Graph WEmbedController::embedGraph(GraphData& graphData) {
        std::map<int, std::set<int>> neighborhoodMap = createNeighborhoodMap(graphData);
        wembed::Graph graph = wembed::graphFromNeighborhoods(neighborhoodMap);
        wembed::Options embedderOptions;
        embedderOptions.embeddingDimension = 3;
        embedderOptions.useWeights = false;
        wembed::Embedder embedder = wembed::createEmbedder(graph, embedderOptions);
        embedder.calculateEmbedding();
        std::vector<std::vector<double>> coordinates = embedder.getCoordinates();

        std::vector<glm::vec3> vertexCoordinates;
        for (auto coord : coordinates) {
            vertexCoordinates.emplace_back(coord[0], coord[1], coord[2]);
        }

        Graph embeddedGraph(vertexCoordinates);

        for (auto edge : graphData.edges) {
            embeddedGraph.addEdge(edge.first, edge.second);
        }

        return embeddedGraph;
    }

    std::map<int, std::set<int>> WEmbedController::createNeighborhoodMap(GraphData& graphData) {
        std::map<int, std::set<int>> neighbors;
        for (int i = 0; i < graphData.vertexCount; i++) {
            neighbors[i] = std::set<int>();
        }

        for (auto edge : graphData.edges) {
            neighbors[edge.first].insert(edge.second);
            neighbors[edge.second].insert(edge.first);
        }

        return neighbors;
    }

}