//
// Created by yannik on 14.01.26.
//

#include "ButtonController.hpp"

#include <filesystem>
#include <iostream>
#include <random>
#include <utility>

#include "ErrorCollector.hpp"
#include "Enums/ImportFormat.hpp"

#define MAX_COLOR_VALUE 1.0f

namespace graphvise {
    ButtonController::ButtonController(Renderer& renderer) : camera(renderer.m_camera()), renderer(renderer),
                                                                             threadController(renderer) {
    }

    void ButtonController::togglePerformanceMode(PerformanceMode mode) {
        renderer.adjustPerformanceMode(mode);
    }

    void ButtonController::randomizeColoring(int groupID) {
        Group& group= GraphSaver::getInstance().getGraph().getGroupByID(groupID);

        std::random_device random;
        std::mt19937 generator(random());
        std::uniform_real_distribution<> distribution(0, MAX_COLOR_VALUE);

        ImVec4 newColor = ImVec4(distribution(generator), distribution(generator), distribution(generator), MAX_COLOR_VALUE);
        group.setColor(newColor);
    }

    void ButtonController::changeColoring(int groupID, ImVec4 newColor) {
        Group& group = GraphSaver::getInstance().getGraph().getGroupByID(groupID);
        group.setColor(newColor);
    }

    void ButtonController::setLightSourceMovementBehaviour(LightSourceMovementBehaviour behaviour) {
        renderer.set_light_source_movement_behaviour(behaviour);
    }

    void ButtonController::setCameraFocusMode(CameraFocusMode mode) {
        camera.set_camera_focus_mode(mode);
    }

    void ButtonController::findVertex(int vertexID) {
        Graph& graph = GraphSaver::getInstance().getGraph();
        if (vertexID >= graph.getVertices().size()) {
            ErrorCollector::getInstance().collectError(Error(ErrorType::NOT_A_VERTEX_ID));
        }

        graph.highlightByID(std::vector{static_cast<uint32_t>(vertexID)}, std::vector<uint32_t>{});
        glm::vec3 vertexPos = graph.getVertexByID(vertexID).getCoordsVector();
        vertexPos.x += 1;
        camera.position_world_space = vertexPos;
        camera.setRotation(0, 3 * std::numbers::pi/2);

        camera.focusPoint = graph.getVertexByID(vertexID).getCoordsVector();
    }

    void ButtonController::findEdge(int firstVertexID, int secondVertexID) {
        Graph& graph = GraphSaver::getInstance().getGraph();
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
            return;
        }

        graph.highlightByID(std::vector<uint32_t>{}, std::vector{edgeID});
        glm::vec3 firstVertexPos = graph.getVertexByID(firstVertexID).getCoordsVector();
        glm::vec3 secondVertexPos = graph.getVertexByID(secondVertexID).getCoordsVector();

        glm::vec3 dirVec = firstVertexPos - secondVertexPos;
        glm::vec3 camOffsetDirVec(dirVec.z, 0, -dirVec.x);

        camOffsetDirVec = normalize(camOffsetDirVec);

        //Calculate center point of the edge
        glm::vec3 averagePos(0, 0, 0);
        averagePos.x += (firstVertexPos.x + secondVertexPos.x) / 2;
        averagePos.y += (firstVertexPos.y + secondVertexPos.y) / 2;
        averagePos.z += (firstVertexPos.z + secondVertexPos.z) / 2;

        glm::vec3 newCamPos = averagePos - camOffsetDirVec;

        camera.position_world_space = newCamPos;

        glm::vec3 toMiddle = normalize(averagePos - newCamPos);

        //I just noticed, I hate LA II
        float newAngle;

        if (toMiddle.x < 0) {
            newAngle = acosf(toMiddle.z) + std::numbers::pi;
        } else {
            newAngle = acosf(-toMiddle.z);
        }
        camera.setRotation(0, newAngle);
        camera.focusPoint = averagePos;
        std::cout << "New Pos: " << newCamPos.x << " " << newCamPos.y << " " << newCamPos.z << std::endl;
    }

    void ButtonController::highlightSubgraph(std::filesystem::path filePath) {
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_SUBGRAPH};
        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::importGraph(std::filesystem::path filePath, ImportFormat importFormat) {

        ThreadOperation threadOperation;

        switch (importFormat)
        {
            case ImportFormat::TXT: threadOperation = {std::move(filePath), ThreadOperationType::PARSE_TXT};
            break;
            case ImportFormat::CNF: threadOperation = {std::move(filePath), ThreadOperationType::PARSE_CNF};
            break;
            default: Error error(ErrorType::INVALID_IMPORT_FORMAT);
                ErrorCollector::getInstance().collectError(error);
                return;
        }

        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::exportGraph(std::filesystem::path filePath, ExportFormat exportFormat) {
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

    void ButtonController::importGroupConfiguration(std::filesystem::path filePath) {
        ThreadOperation threadOperation = {std::move(filePath), ThreadOperationType::PARSE_GROUPS};

        if (!threadController.notifyBackgroundThread(threadOperation)) {
            Error error(ErrorType::BACKGROUND_THREAD_ALREADY_BUSY);
            ErrorCollector::getInstance().collectError(error);
        }
    }

    void ButtonController::changeTransparency(const uint32_t groupID, const float newTransparency)
    {
        GraphSaver::getInstance().getGraph().setGroupTransparency(groupID, newTransparency);
    }

    void ButtonController::RemoveHighlights() {
        GraphSaver::getInstance().getGraph().removeAllHighlights();
        camera.resetFocusPoint();
    }

    void ButtonController::addCurrentPosAsBookmark(const std::string& name)
    {
        GraphSaver::getInstance().getGraph().addCameraBookmark(name, camera.position_world_space, camera.rotation_x, camera.rotation_y);
    }

    void ButtonController::loadCameraBookmark(CameraBookmark cam)
    {
        camera.position_world_space = cam.getCoordsVector();
        camera.rotation_x = cam.getPitch();
        camera.rotation_y = cam.getYaw();
    }
}
