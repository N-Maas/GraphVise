//
// Created by yannik on 14.01.26.
//

#include "ButtonController.hpp"

#include <random>
#include <utility>

#include "ErrorCollector.hpp"

#define MAX_COLOR_VALUE 255

namespace graphvise {
    ButtonController::ButtonController(Camera& camera, Renderer& renderer) : camera(camera), renderer(renderer), threadController(renderer) {
    }

    void ButtonController::togglePerformanceMode(PerformanceMode mode) {
        //TODO: Implementieren, wenn es eine entsprechende Methode im Renderer gibt
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
        //TODO: Implement, when function for highlighting single Vertex exists
    }

    void ButtonController::findEdge(int firstVertexID, int secondVertexID) {
        std::optional<uint32_t>  edgeID = GraphSaver::getGraphSaver().getGraph().getEdgeIDByConnectingVerticesIDs(firstVertexID, secondVertexID);
        //TODO: Implement, when function for highlighting single Edge exists
    }

    void ButtonController::highlightSubgraph(std::string filePath) {
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_SUBGRAPH};
        threadController.notifyBackgroundThread(threadOperation);
    }

    void ButtonController::importGraph(std::string filePath) {
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_TXT};
        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::exportGraph(std::string filePath, ExportFormat exportFormat) {
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