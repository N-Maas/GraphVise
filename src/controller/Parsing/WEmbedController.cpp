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

    model::Graph embeddedGraph(graphData.vertexCount);

    for (int i = 0; i < coordinates.size(); i++) {
        glm::vec3 convertedVector(coordinates[i][0], coordinates[i][1], coordinates[i][2]);
        embeddedGraph.addVertex(i, convertedVector);
    }

    for (auto edge : graphData.edges) {
        embeddedGraph.addEdge(edge.first, edge.second);
    }

    return embeddedGraph;
}
