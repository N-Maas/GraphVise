//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include "imgui/imgui.h"
#include "imfilebrowser.h"
#include "rendering/renderer.hpp"
#include "controller/ButtonController.hpp"

namespace graphvise
{
    class Buttons
    {
    public:
        Buttons(ButtonController *controller);

        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:

        const std::vector<std::string> allowedFiles = {".txt"};
        PerformanceMode mode = HIGH_PERFORMANCE;

        GraphSaver *saver = &GraphSaver::getGraphSaver();
        std::vector<Group>* groups = nullptr;

        ButtonController *buttonController;
        uint8_t performanceModeSlider = 0;
        uint32_t framebufferWidth = 0;
        uint32_t framebufferHeight = 0;

        int32_t vertex = 0;
        int32_t edgeVertices[2] = {0, 0};

        ImGui::FileBrowser importGraphBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser importGroupConfigBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser highlightSubgraphBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser exportGraphBrowser = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);

        void MainMenuBar();
        void GroupMenu(bool* groupMenu);
        void findObject(bool* findObject);
        void performanceModeToggle(bool* performanceMode);
        void randomizeColoring(int groupID);
        void changeColoring(int groupID);
        void toggleLightSourceMovement(bool* lightSourceMovement);
        void setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviour);
        void setCameraMovementMode(bool* cameraMovementMode);
        void findVertex();
        void findEdge();
        void highlightSubgraph();
        void importGraph();
        void exportGraph();
        void SideBar();
        void importGroupConfiguration();
    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP