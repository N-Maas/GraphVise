//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"
#include "AdamOptimizer.hpp"
#include "wembed.h"

#define MAX_SCALE 10
#define START_VALUE .5
#define EDGE_GROWTH_FACTOR 0.001
#define VERTEX_GROWTH_FACTOR 0.1

namespace graphvise {
    Graph WEmbedController::embedGraph(GraphData& graphData) {
        std::map<int, std::set<int>> neighborhoodMap = createNeighborhoodMap(graphData);
        wembed::Graph graph = wembed::graphFromNeighborhoods(neighborhoodMap);
        wembed::Options embedderOptions;
        embedderOptions.embeddingDimension = 3;
        embedderOptions.useWeights = false;
        embedderOptions.maxIterations = 3000;
        embedderOptions.layeredEmbedding = true;
        wembed::Embedder embedder = wembed::createEmbedder(graph, embedderOptions);
        embedder.calculateEmbedding();
        std::vector<std::vector<double>> coordinates = embedder.getCoordinates();

        std::vector<glm::vec3> vertexCoordinates;
        for (auto coord : coordinates) {
            vertexCoordinates.emplace_back(coord[0], coord[1], coord[2]);
        }

        int numberOfIgnoredVertices = 0;
        glm::vec3 average(0, 0, 0);
        for (int vertexID = 0; vertexID < vertexCoordinates.size(); vertexID++) {
            if (neighborhoodMap[vertexID].size() == 0) {
                numberOfIgnoredVertices++;
                continue;
            }
            average.x += vertexCoordinates[vertexID].x;
            average.y += vertexCoordinates[vertexID].y;
            average.z += vertexCoordinates[vertexID].z;
        }
        average.x /= vertexCoordinates.size() - numberOfIgnoredVertices;
        average.y /= vertexCoordinates.size() - numberOfIgnoredVertices;
        average.z /= vertexCoordinates.size() - numberOfIgnoredVertices;

        //Max Scale amount is based on 10th root of vertex count and edge count
        float maxEdgeScale = 5 * pow(graphData.edges.size(), static_cast<float>(1)/10);
        float maxVertexScale = 3 * pow(graphData.vertexCount * 10, static_cast<float>(1)/10);

        //Calculating scaling factor based on logistic growth
        float vertexScaleFactor = maxVertexScale * (1/(1+pow(std::numbers::e, -VERTEX_GROWTH_FACTOR*maxVertexScale*graphData.vertexCount)*((maxVertexScale/START_VALUE)-1)));
        float edgeScaleFactor = maxEdgeScale * (1/(1+pow(std::numbers::e, -EDGE_GROWTH_FACTOR*maxEdgeScale*graphData.edges.size())*((maxEdgeScale/START_VALUE)-1)));

        //Moving center of mass to (0, 0, 0) and scaling graph
        for (auto& vertex : vertexCoordinates) {
            vertex.x -= average.x;
            vertex.y -= average.y;
            vertex.z -= average.z;
            vertex.x += vertexScaleFactor * edgeScaleFactor * vertex.x;
            vertex.y += vertexScaleFactor * edgeScaleFactor * vertex.y;
            vertex.z += vertexScaleFactor * edgeScaleFactor * vertex.z;
        }

        std::vector<std::pair<std::uint32_t, std::uint32_t>> edgeConnectingVertices;
        for (auto edge : graphData.edges) {
            edgeConnectingVertices.emplace_back(edge.first, edge.second);
        }

        Graph embeddedGraph(vertexCoordinates, edgeConnectingVertices, graphData.name);

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