//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"

#include "AdamOptimizer.hpp"
#include "wembed.h"

#define MAX_SCALE 8
#define START_VALUE 0.1
#define GROWTH_FACTOR 0.02

namespace graphvise {
    Graph WEmbedController::embedGraph(GraphData& graphData) {
        std::map<int, std::set<int>> neighborhoodMap = createNeighborhoodMap(graphData);
        wembed::Graph graph = wembed::graphFromNeighborhoods(neighborhoodMap);
        wembed::Options embedderOptions;
        embedderOptions.embeddingDimension = 3;
        embedderOptions.useWeights = false;
        embedderOptions.maxIterations = 3000;
        wembed::Embedder embedder = wembed::createEmbedder(graph, embedderOptions);
        embedder.calculateEmbedding();
        std::vector<std::vector<double>> coordinates = embedder.getCoordinates();

        std::vector<glm::vec3> vertexCoordinates;
        for (auto coord : coordinates) {
            vertexCoordinates.emplace_back(coord[0], coord[1], coord[2]);
        }

        glm::vec3 average;
        for (auto vertex : vertexCoordinates) {
            average.x += vertex.x / vertexCoordinates.size();
            average.y += vertex.y / vertexCoordinates.size();
            average.z += vertex.z / vertexCoordinates.size();
        }


        float vertexScaleFactor = MAX_SCALE * (1/(1+pow(std::numbers::e, -GROWTH_FACTOR*MAX_SCALE*graphData.vertexCount)*((MAX_SCALE/START_VALUE)-1)));
        float edgeScaleFactor = MAX_SCALE * (1/(1+pow(std::numbers::e, -GROWTH_FACTOR*MAX_SCALE*graphData.edges.size())*((MAX_SCALE/START_VALUE)-1)));

        /*float vertexScaleFactor = (pow(graphData.vertexCount * 0.04, static_cast<float>(1)/3));
        float edgeScaleFactor = (pow(graphData.edges.size(), static_cast<float>(1)/6));*/

        for (auto& vertex : vertexCoordinates) {
            vertex.x += vertexScaleFactor * edgeScaleFactor * (vertex.x - average.x);
            vertex.y += vertexScaleFactor * edgeScaleFactor * (vertex.y - average.y);
            vertex.z += vertexScaleFactor * edgeScaleFactor * (vertex.z - average.z);
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