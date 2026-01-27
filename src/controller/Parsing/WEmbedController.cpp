//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"

#include "wembed.h"

Graph WEmbedController::embedGraph(GraphData& graphData) {
    wembed::Graph graph = wembed::graphFromEdges(graphData.edges);
    wembed::Options embedderOptions;
    embedderOptions.embeddingDimension = 3;
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
