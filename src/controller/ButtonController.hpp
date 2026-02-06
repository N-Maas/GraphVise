//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP
#define THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP


#include "ThreadController.hpp"
#include "../rendering/renderer.hpp"
#include "../rendering/camera.hpp"
#include "Enums/ExportFormat.hpp"
#include "Enums/ImportFormat.hpp"
#include "imgui/imgui.h"

namespace graphvise {
    class ButtonController {
    public:
        ButtonController(Renderer& renderer);
        void togglePerformanceMode(PerformanceMode mode);
        void randomizeColoring(int groupID);
        void changeColoring(int groupID, ImVec4 color);
        void toggleLightSourceMovement();
        void setLightSourceMovementBehaviour(LightSourceMovementBehaviour behaviour);
        void setCameraFocusMode(CameraFocusMode mode);
        void findVertex(int vertexID);
        void findEdge(int firstVertexID, int secondVertexID);
        void highlightSubgraph(std::string filePath);
        void importGraph(std::string filePath, ImportFormat importFormat);
        void exportGraph(std::string filePath, ExportFormat format);
        void importGroupConfiguration(std::string filePath);
        void changeTransparency(uint32_t groupID, float newTransparency);
        void RemoveHighlights();
        void addCurrentPosAsBookmark(const std::string& name);
        void loadCameraBookmark(CameraBookmark cam);

    private:
        Camera& camera;
        Renderer& renderer;
        ThreadController threadController;
    };
}

#endif //THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP