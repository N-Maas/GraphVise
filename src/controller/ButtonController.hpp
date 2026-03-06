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
        explicit ButtonController(const std::shared_ptr<Renderer>& renderer);
        void togglePerformanceMode();
        void setPerformanceMode(PerformanceMode mode);
        void randomizeColoring(int groupID);
        void changeColoring(uint32_t groupID, ImVec4 newColor);
        void setLightSourceMovementBehaviour(LightSourceMovementBehaviour behaviour);
        void toggleLightSourceMovementBehaviour();
        void setCameraFocusMode(CameraFocusMode mode);
        void toggleCameraFocusMode();
        void findVertex(uint32_t vertexID);
        void findEdge(int firstVertexID, int secondVertexID);
        void highlightSubgraph(std::filesystem::path filePath);
        void importGraph(std::filesystem::path filePath, ImportFormat importFormat);
        void exportGraph(std::filesystem::path filePath, ExportFormat exportFormat);
        void importGroupConfiguration(std::filesystem::path filePath);
        void changeTransparency(uint32_t groupID, float newTransparency);
        void RemoveHighlights();
        void addCurrentPosAsBookmark(const std::string& name);
        void loadCameraBookmark(CameraBookmark cam);
        void deleteCameraBookmark(size_t bookmarkID);
        void randomizeAllColors();
        void scaleGraph(float scale);

    private:
        Camera& camera;
        std::shared_ptr<Renderer> renderer;
        ThreadController threadController;
    };
}

#endif //THESIS_FRAMEWORK_BUTTONCONTROLLER_HPP