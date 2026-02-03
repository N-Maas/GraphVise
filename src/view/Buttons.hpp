//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include "imgui/imgui.h"
#include "imfilebrowser.h"
#include "controller/ButtonController.hpp"

namespace graphvise
{
    class Buttons
    {
    public:
        explicit Buttons(ButtonController *controller);

        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:

        const std::vector<std::string> allowedFiles = {".txt"};


        GraphSaver *saver = &GraphSaver::getGraphSaver();
        const std::vector<Group>* activeGroups = nullptr;
        ButtonController *buttonController;

        PerformanceMode performanceMode = Renderer::getInstance().get()->performance_mode();
        CameraFocusMode cameraMode = Renderer::getInstance().get()->m_camera().camera_focus_mode();
        LightSourceMovementBehaviour lightSourceMovementBehaviour = Renderer::getInstance().get()->light_source_movement_behaviour();

        std::vector<ImVec4> groupColors = std::vector<ImVec4>(16);

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
        void ChangeTransparency(uint32_t groupID);
        void findObject(bool* findObject);
        void performanceModeToggle(bool* toggle_mode);
        void randomizeColoring(uint32_t groupID) const;
        void changeColoring(uint32_t groupID);
        void setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviorToggle);
        void setCameraMovementMode(bool* cameraMovementMode);
        void findVertex();
        void findEdge();
        void highlightSubgraph();
        void importGraph();
        void exportGraph();
        void SideBar();
        static void SideBarElement(const char* label, bool* state);
        void importGroupConfiguration();
    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP