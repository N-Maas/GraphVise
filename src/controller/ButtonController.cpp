//
// Created by yannik on 14.01.26.
//

#include "ButtonController.hpp"

#include <iostream>
#include <random>
#include <utility>

#include "ErrorCollector.hpp"

#define MAX_COLOR_VALUE 1.0f

namespace graphvise {
    ButtonController::ButtonController(Renderer& renderer) : camera(renderer.m_camera()), renderer(renderer),
                                                                             threadController(renderer) {
    }

    void ButtonController::togglePerformanceMode(PerformanceMode mode) {
        renderer.adjustPerformanceMode(mode);
    }

    void ButtonController::randomizeColoring(int groupID) {
        Group& group= GraphSaver::getGraphSaver().getGraph().getGroupByID(groupID);

        std::random_device random;
        std::mt19937 generator(random());
        std::uniform_real_distribution<> distribution(0, MAX_COLOR_VALUE);

        ImVec4 newColor = ImVec4(distribution(generator), distribution(generator), distribution(generator), MAX_COLOR_VALUE);
        group.setGroupVec4(newColor);
    }

    void ButtonController::changeColoring(int groupID, ImVec4 newColor) {
        Group& group = GraphSaver::getGraphSaver().getGraph().getGroupByID(groupID);
        group.setGroupVec4(newColor);
    }

    void ButtonController::toggleLightSourceMovement() {
        //TODO: Implement, when movement for light source is implemented
    }

    void ButtonController::setLightSourceMovementBehaviour(LightSourceMovementBehaviour behaviour) {
        //TODO: Implement, when movement for light source is implemented
    }

    void ButtonController::setCameraFocusMode(CameraFocusMode mode) {
        //TODO: Implement, when movement for camera is implemented
    }

    void ButtonController::findVertex(int vertexID) {
        Graph& graph = GraphSaver::getGraphSaver().getGraph();
        if (vertexID < graph.getVertices().size()) {
            graph.highlightByID(std::vector{static_cast<uint32_t>(vertexID)}, std::vector<uint32_t>{});
            glm::vec3 vertexPos = graph.getVertexByID(vertexID).getCoordsVector();
            vertexPos.x += 1;
            camera.position_world_space = vertexPos;
            camera.setRotation(0, 3 * std::numbers::pi/2);
        } else {
            ErrorCollector::getInstance().collectError(Error(ErrorType::NOT_A_VERTEX_ID));
        }
    }

    void ButtonController::findEdge(int firstVertexID, int secondVertexID) {
        Graph& graph = GraphSaver::getGraphSaver().getGraph();
        uint32_t maxVertexID = graph.getVertices().size() - 1;

        if (firstVertexID == secondVertexID) {
            ErrorCollector::getInstance().collectError(Error(ErrorType::EQUAL_VERTEX_IDS));
            return;
        }
        if (firstVertexID > maxVertexID) {
            ErrorCollector::getInstance().collectError(Error(ErrorType::VERTEX_ID_OUT_OF_BOUNDS, std::to_string(firstVertexID)));
            return;
        }
        if (secondVertexID > maxVertexID) {
            ErrorCollector::getInstance().collectError(Error(ErrorType::VERTEX_ID_OUT_OF_BOUNDS, std::to_string(secondVertexID)));
            return;
        }

        uint32_t edgeID;
        try {
            edgeID = graph.getEdgeIDByConnectingVerticesIDs(firstVertexID, secondVertexID);
        } catch (std::out_of_range& e) {
            ErrorCollector::getInstance().collectError(Error(ErrorType::EDGE_DOES_NOT_EXIST));
        }

        graph.highlightByID(std::vector<uint32_t>{}, std::vector{edgeID});
        glm::vec3 firstVertexPos = graph.getVertexByID(firstVertexID).getCoordsVector();
        glm::vec3 secondVertexPos = graph.getVertexByID(secondVertexID).getCoordsVector();

        glm::vec3 dirVec = firstVertexPos - secondVertexPos;
        glm::vec3 camOffsetDirVec(dirVec.z, 0, -dirVec.x);

        camOffsetDirVec = normalize(camOffsetDirVec);

        //Calculate center point of the edge
        glm::vec3 averagePos;
        averagePos.x += (firstVertexPos.x + secondVertexPos.x) / 2;
        averagePos.y += (firstVertexPos.y + secondVertexPos.y) / 2;
        averagePos.z += (firstVertexPos.z + secondVertexPos.z) / 2;

        glm::vec3 newCamPos = averagePos - camOffsetDirVec;

        camera.position_world_space = newCamPos;

        glm::vec3 toMiddle = normalize(averagePos - newCamPos);

        //I just noticed, I hate LA II
        float newAngle;

        if (toMiddle.x < 0) {
            newAngle = acos(toMiddle.z) + std::numbers::pi;
        } else {
            newAngle = acos(-toMiddle.z);
        }
        camera.setRotation(0, newAngle);
    }

    void ButtonController::highlightSubgraph(std::string filePath) {
        //TODO: Finish when GUI and ThreadController are ready
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_SUBGRAPH};
        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::importGraph(std::string filePath) {
        //TODO: Finish when GUI and ThreadController are ready
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_TXT};
        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::exportGraph(std::string filePath, ExportFormat exportFormat) {
        //TODO: Finish when GUI and ThreadController are ready
        ThreadOperationType operationType;
        switch (exportFormat) {
            case ExportFormat::PNG: operationType = ThreadOperationType::EXPORT_PNG;
                break;

            default: throw std::invalid_argument("ExportFormat not defined!");
        }
        ThreadOperation threadOperation = {std::move(filePath), operationType};

        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::importGroupConfiguration(std::string filePath) {
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_GROUPS};

        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }
}