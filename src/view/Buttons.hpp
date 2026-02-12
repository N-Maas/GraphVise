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
        explicit Buttons(ButtonController *controller);

        void loadButtonFrame(int framebufferWidth, int framebufferHeight);

    private:


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



        ImportFormat importFormat = ImportFormat::TXT;
        ExportFormat exportFormat = ExportFormat::PNG;

        GraphSaver *saver = &GraphSaver::getInstance();
        const std::vector<Group>* activeGroups = nullptr;
        ButtonController *buttonController;

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

        Texture cameraBookmarkIcon = loadTextureFromFile(ICON_FILE_PATH "bookmark.png");
        Texture randomize = loadTextureFromFile(ICON_FILE_PATH "Randomize Color button.png");
        Texture searchIcon = loadTextureFromFile(ICON_FILE_PATH "Suche.png");
        Texture groupIcon = loadTextureFromFile(ICON_FILE_PATH "Gruppen.png");
        Texture performanceIcon = loadTextureFromFile(ICON_FILE_PATH "Performance.png");
        Texture cameraMovementIcon = loadTextureFromFile(ICON_FILE_PATH "cameraMovement.png");
        Texture lightSourceIcon = loadTextureFromFile(ICON_FILE_PATH "Light Source Switch Button.png");

        Texture loadTextureFromFile(const char* filename);

        void changeObjSize();
        void graphSettings();
        void MainMenuBar();
        void GroupMenu(bool* groupMenu);
        void ChangeTransparency(uint32_t groupID);
        void findObject();
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
        static void SideBarElement(Texture texture, const char* hoverMsg, bool* state);
        void importGroupConfiguration();
    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP