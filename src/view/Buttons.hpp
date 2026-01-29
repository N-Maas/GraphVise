//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include "imgui/imgui.h"
#include "imfilebrowser.h"

namespace graphvise
{
    class Buttons
    {
    public:
        Buttons(int buttonController);

        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:
        const std::vector<std::string> allowedFiles = {".txt"};
        int buttonController = 0;
        int performanceModeSlider = 0;
        int framebufferWidth = 0;
        int framebufferHeight = 0;

        int vertex = -1;
        int edge = -1;

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