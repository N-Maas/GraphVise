//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP
#define THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP
#include "ThreadController.hpp"
#include "../rendering/camera.hpp"
#include "../rendering/renderer.hpp"
#include "Enums/ExportFormat.hpp"
#include "imgui/imgui.h"


class ButtonController {
    public:
        ButtonController(Camera& camera, Renderer& renderer);
        void togglePerformanceMode(PerformanceMode mode);
        void randomizeColoring(int group);
        void changeColoring(int group, ImVec4 color);
        void toggleLightSourceMovement();
        void setLightSourceMovementBehaviour(LightSourceMovementBehaviour behaviour);
        void setCameraMovementMode(CameraFocusMode mode);
        void findVertex(int id);
        void findEdge(int firstVertex, int secondVertex);
        void highlightSubgraph(std::string filePath);
        void importGraph(std::string filePath);
        void exportGraph(std::string filePath, ExportFormat format);
        void importGroupConfiguration(std::string filePath);

    private:
        Camera& camera;
        Renderer& renderer;
        ThreadController threadController;
};


#endif //THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP