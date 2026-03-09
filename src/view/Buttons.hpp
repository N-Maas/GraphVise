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
#include "model/GraphSaver.hpp"

#define ICON_FILE_PATH "../ext/GUIIcons/"


namespace graphvise
{
    struct Texture
    {
        ImTextureID id = nullptr;
        int width = 0;
        int height = 0;
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

        bool addBookmarkWindow = false;

        ImportFormat importFormat = ImportFormat::TXT;
        ExportFormat exportFormat = ExportFormat::PNG;

        GraphSaver *saver = &GraphSaver::getInstance();
        const std::vector<Group>* activeGroups = nullptr;

        std::shared_ptr<ButtonController> buttonController;

        const PerformanceMode* performanceMode = Renderer::getInstance().get()->performance_mode();
        const CameraFocusMode* cameraMode = Renderer::getInstance().get()->m_camera().camera_focus_mode();
        const LightSourceMovementBehaviour* lightSourceMovementBehaviour = Renderer::getInstance().get()->light_source_movement_behaviour();

        std::vector<std::pair<ImVec4,ImVec4>> groupColors = std::vector<std::pair<ImVec4,ImVec4>>(16);

        std::vector<char> bookmarkName = std::vector<char>(16);

        uint32_t framebufferWidth = 0;
        uint32_t framebufferHeight = 0;


        int32_t vertex = 0;
        int32_t edgeVertices[2] = {0, 0};


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
        Texture cameraMovementIcon2;
        Texture lightSourceIcon;

        void MainMenuBar();

        void SideBar();
        void SideBarElement(Texture texture, const char* hoverMsg, const std::function<void(const char* hoverMsg)>& onClickFunction);

        void changeObjSize();
        void scaleGraph();
        void graphSettings();
        void generalSettings();
        void GroupMenu(const char* popUpName);
        void ChangeTransparency(uint32_t groupID);
        void findObject(const char* popUpName);
        void performanceModeToggle(const char* popUpName);
        void randomizeColoring(uint32_t groupID) const;
        void changeColoring(uint32_t groupID);
        void findVertex();
        void findEdge();
        void highlightSubgraph();
        void importGraph();
        void exportGraph();
        void cameraBookmarkMenu(const char* popUpName);
        void importGroupConfiguration();
        void help();
        static Texture loadTextureFromFile(const char* filename);

    };
}

#endif //THESIS_FRAMEWORK_BUTTONS_HPP