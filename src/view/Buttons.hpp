//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include <functional>

#include "imgui/imgui.h"
#include "imfilebrowser.h"
#include "controller/ButtonController.hpp"

#define ICON_FILE_PATH "../ext/GUIIcons/"


namespace graphvise
{
    struct Texture
    {
        ImTextureID id;
        int width;
        int height;
    };

    class Buttons
    {
    public:
        explicit Buttons(std::shared_ptr<ButtonController> buttonController);
        void initButtons();
        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:

        std::shared_ptr<Renderer> renderer = Renderer::getInstance();

        const std::vector<std::string> allowedGroupInfoFormat = {".txt"};
        const std::vector<std::string> txtImportFormat = {".txt"};
        const std::vector<std::string> cnfImportFormat = {".cnf"};
        const std::vector<std::string> allowedExportFormat = {".png"};


        bool search = false;
        bool groups = false;
        bool togglePerformanceMode = false;
        bool lightSource = false;
        bool cameraMovement = false;
        bool cameraBookmarks = false;
        bool addBookmarkWindow = false;

        bool* currentBoolPtr = nullptr;

        ImportFormat importFormat = ImportFormat::TXT;
        ExportFormat exportFormat = ExportFormat::PNG;

        GraphSaver *saver = &GraphSaver::getInstance();
        const std::vector<Group>* activeGroups = nullptr;

        std::shared_ptr<ButtonController> buttonController;

        std::function<void()> currentSideBar = nullptr;

        PerformanceMode performanceMode = Renderer::getInstance().get()->performance_mode();
        CameraFocusMode cameraMode = Renderer::getInstance().get()->m_camera().camera_focus_mode();
        LightSourceMovementBehaviour lightSourceMovementBehaviour = Renderer::getInstance().get()->light_source_movement_behaviour();

        std::vector<ImVec4> groupColors = std::vector<ImVec4>(16);

        std::vector<char> bookmarkName = std::vector<char>(16);

        uint32_t framebufferWidth = 0;
        uint32_t framebufferHeight = 0;

        int32_t vertex = 0;
        int32_t edgeVertices[2] = {0, 0};

        float cylinderRadius = Renderer::getInstance()->getCylinderRadius();
        float sphereRadius = Renderer::getInstance()->getSphereRadius();


        ImGui::FileBrowser importGraphBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser importGroupConfigBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser highlightSubgraphBrowser = ImGui::FileBrowser();
        ImGui::FileBrowser exportGraphBrowser = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);

        Texture cameraBookmarkIcon;
        Texture randomize;
        Texture searchIcon;
        Texture groupIcon;
        Texture performanceIcon;
        Texture cameraMovementIcon;
        Texture lightSourceIcon;

        Texture loadTextureFromFile(const char* filename);

        void changeObjSize();
        void graphSettings();
        void generalSettings();
        void MainMenuBar();
        void GroupMenu();
        void ChangeTransparency(uint32_t groupID);
        void findObject();
        void performanceModeToggle();
        void randomizeColoring(uint32_t groupID) const;
        void changeColoring(uint32_t groupID);
        void setLightSourceMovementBehaviour();
        void setCameraMovementMode();
        void findVertex();
        void findEdge();
        void highlightSubgraph();
        void importGraph();
        void exportGraph();
        void cameraBookmarkMenu();
        void SideBar();
        void SideBarElement(Texture texture, const char* hoverMsg, const std::function<void()>& onClickFunction);
        void importGroupConfiguration();
    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP