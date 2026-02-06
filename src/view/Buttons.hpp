//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include "imgui/imgui.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include "model/Group.hpp"
#include "rendering/renderer.hpp"
#include "../rendering/enums.hpp"
#include "controller/ButtonController.hpp"
#include "model/GraphSaver.hpp"

namespace graphvise
{
    class Buttons
    {
    public:
        explicit Buttons(ButtonController *controller);

        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:

        bool search = false;
        bool groups = false;
        bool togglePerformanceMode = false;
        bool lightSource = false;
        bool cameraMovement = false;
        bool cameraBookmarks = false;
        bool addBookmarkWindow = false;

        const std::vector<std::string> allowedFiles = {".txt"};

        // Helper function to get renderer (defined inline)
        static Renderer* getRenderer() {
            auto instance = Renderer::getInstance();
            return instance ? instance.get() : nullptr;
        }

        // Initialize using helper function
        GraphSaver *saver = &graphvise::GraphSaver::getInstance();
        ButtonController *buttonController;

        // Use ternary operator to handle nullptr
        PerformanceMode performanceMode = getRenderer() ? getRenderer()->performance_mode() : PerformanceMode::BALANCE;
        CameraFocusMode cameraMode = getRenderer() ? getRenderer()->m_camera().camera_focus_mode() : CameraFocusMode::CENTER_OF_MASS;
        LightSourceMovementBehaviour lightSourceMovementBehaviour = getRenderer() ? getRenderer()->light_source_movement_behaviour() : LightSourceMovementBehaviour::FIXED_POSITION;

        const std::vector<Group>* activeGroups = nullptr;

        std::vector<ImVec4> groupColors = std::vector<ImVec4>(16);

        std::vector<char> bookmarkName = std::vector<char>(16);

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
        void cameraBookmarkMenu(bool* visible);
        void SideBar();
        static void SideBarElement(const char* label, bool* state);
        void importGroupConfiguration();
    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP