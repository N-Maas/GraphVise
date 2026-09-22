//
// Created by yannik on 23.01.26.
//

#include "WEmbedController.hpp"

#include <algorithm>
#include "wembed.h"

#define MAX_SCALE 10
#define START_VALUE .5
#define EDGE_GROWTH_FACTOR 0.001
#define VERTEX_GROWTH_FACTOR 0.1

namespace graphvise {
    Graph WEmbedController::embedGraph(GraphData& graphData) {
        std::map<int, std::set<int>> neighborhoodMap = createNeighborhoodMap(graphData);
        std::vector<wembed::Edge> edges;
        edges.reserve(graphData.edges.size());
        for (auto edge : graphData.edges) {
            edges.push_back({static_cast<wembed::NodeId>(edge.first), static_cast<wembed::NodeId>(edge.second)});
        }
        // NOTE(JP) wembed derives the number of vertices from the largest id that has an edge, so isolated vertices
        // at the end of the id range would not be embedded. To get a position for every vertex (like before),
        // the largest id that has an edge takes the place of the last id. Ids without edges below the largest
        // one are filled up by wembed. The swap is undone on the coordinates below.
        // In the future, wembed should really readd the GraphFromNeighborhood function
        wembed::NodeId lastId = static_cast<wembed::NodeId>(graphData.vertexCount) - 1;
        wembed::NodeId largestUsedId = -1;
        for (const auto& edge : edges) {
            largestUsedId = std::max({largestUsedId, edge.src, edge.dst});
        }
        for (auto& edge : edges) {
            if (edge.src == largestUsedId) edge.src = lastId;
            if (edge.dst == largestUsedId) edge.dst = lastId;
        }
        wembed::Graph graph = wembed::graphFromEdges(edges);
        wembed::Options embedderOptions;
        embedderOptions.embeddingDimension = 3;
        embedderOptions.useUnitWeights = true;
        embedderOptions.layeredEmbedding = true;
        // The KD-tree is always available, the sprk index needs wembed to be built with Rust.
        embedderOptions.indexType = wembed::IndexKdTree;

        // NOTE(JP) These values reproduce the behavior of the old wembed branch (cpp-library-interface)
        // By now Wembed uses StopLoss as the default stop criterion and also changed some other defaults.
        // It would require some parameter tuning to get comparable runtime and quality with StopLoss.
        embedderOptions.optimizerType = wembed::OptimizerAdam;
        embedderOptions.lrSchedule = wembed::LRExponentialCooling;
        embedderOptions.learningRate = 10.0;
        embedderOptions.lrCoolingFactor = 0.99;
        embedderOptions.maxIterations = 3000; 
        embedderOptions.stopCriterion = wembed::StopDisplacement;
        // Relative movement per step below which the layout counts as settled. Stricter than wembed's default
        // (3e-4), which stops too early on instances with many isolated variables or components.
        embedderOptions.stopDisplacementTol = 1e-4;
        embedderOptions.stopDisplacementPatience = 5;
        wembed::Embedder embedder = wembed::createEmbedder(graph, embedderOptions);
        embedder.calculateEmbedding();
        std::vector<std::vector<double>> coordinates = embedder.getCoordinates();

        std::vector<glm::vec3> vertexCoordinates;
        for (auto coord : coordinates) {
            vertexCoordinates.emplace_back(coord[0], coord[1], coord[2]);
        }
        // only needed for a graph without any edge, otherwise wembed returned a position for every vertex
        vertexCoordinates.resize(graphData.vertexCount, glm::vec3(0, 0, 0));
        // undo the swap of the largest id and the last id
        if (largestUsedId >= 0 && largestUsedId != lastId) {
            std::swap(vertexCoordinates[largestUsedId], vertexCoordinates[lastId]);
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
        // NOTE(JP) I think moving the center of mass to (0,0,0) should be done automatically by wembed now.
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