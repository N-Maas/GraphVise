//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"

#include "Graph.hpp"
#include "../../../cmake-build-release/_deps/wembed-src/src/embeddingLib/include/embedder/WEmbedEmbedder.hpp"

model::Graph WEmbedController::embedGraph(GraphData& graphData) {
    Graph graph(graphData.edges);
    EmbedderOptions embedderOptions;
    embedderOptions.embeddingDimension = 3;
    WEmbedEmbedder embedder(graph, embedderOptions);
    embedder.calculateEmbedding();
    std::vector<std::vector<double>> coordinates = embedder.getCoordinates();

    std::vector<glm::vec3> vertexCoordinates;
    for (auto coord : coordinates) {
        vertexCoordinates.emplace_back(coord[0], coord[1], coord[2]);
    }

    model::Graph embeddedGraph(vertexCoordinates);

    for (auto edge : graphData.edges) {
        embeddedGraph.addEdge(edge.first, edge.second);
    }

    return embeddedGraph;
}
