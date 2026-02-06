//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"

#include <format>

#include "imgui/imgui.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include "../rendering/enums.hpp"
#include "../model/GraphSaver.hpp"
#include "../model/Graph.hpp"
#include "../model/Group.hpp"

#define MainMenuBarHeight 19
#define findObjectHeight 100
#define GroupMenuHeight 300
#define PerformanceHeight 54
#define MovementLightSourceHeight 77
#define MovementCameraHeight 77

namespace graphvise
{
    Buttons::Buttons(ButtonController *controller)
    {
        this->buttonController = controller;

        importGraphBrowser.SetTypeFilters(allowedFiles);
        importGroupConfigBrowser.SetTypeFilters(allowedFiles);
        highlightSubgraphBrowser.SetTypeFilters(allowedFiles);
    }

    void Buttons::loadButtonFrame(int framebufferWidth,int framebufferHeight)
    {
        this->framebufferWidth = framebufferWidth;
        this->framebufferHeight = framebufferHeight;


        static float cylinderRadius = 0.03f;
        static float sphereRadius = 0.05f;

        if (ImGui::DragFloat("Edge Size", &cylinderRadius, 0.001f, 0.001f, 1.0f))
        {
            Renderer::getInstance()->setCylinderRadius(cylinderRadius);
        };
        if (ImGui::DragFloat("Vertex Size", &sphereRadius, 0.001f, 0.001f, 1.0f))
        {
            Renderer::getInstance()->setSphereRadius(sphereRadius);
        }



        // ImGui::ShowDemoWindow();

        MainMenuBar();

        SideBar();


    }

    void Buttons::MainMenuBar()
    {
        // Main Menu Bar at the top of the Window
        ImGui::BeginMainMenuBar();


        importGraph();
        ImGui::Separator();
        importGroupConfiguration();
        ImGui::Separator();
        highlightSubgraph();
        ImGui::Separator();
        exportGraph();
        ImGui::Separator();


        ImGui::EndMainMenuBar();

        importGraphBrowser.Display();
        importGroupConfigBrowser.Display();
        highlightSubgraphBrowser.Display();
        exportGraphBrowser.Display();

    }

    void Buttons::SideBar()
    {

        ImVec2 pos;
        pos.x = static_cast<float>(framebufferWidth);
        pos.y = static_cast<float>(framebufferHeight) / 2.0f;

        ImVec2 windowPivot = {1.0f, 0.5f};

        ImGui::SetNextWindowPos(pos, 0, windowPivot);


        ImGui::Begin("##SideBarMenu", nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoTitleBar
        );

        SideBarElement("Search Object", &search);
        SideBarElement("Groups", &groups);
        SideBarElement("Performance Mode", &togglePerformanceMode);
        SideBarElement("Light Source Behaviour", &lightSource);
        SideBarElement("Camera Movement Mode", &cameraMovement);
        SideBarElement("Camera bookmarks", &cameraBookmarks);


        const ImVec2 sideBarSize = ImGui::GetWindowSize();
        ImGui::End();

        float widgetSpacing = 3.0f;

        pos.x = pos.x - sideBarSize.x;
        pos.y = MainMenuBarHeight;

        windowPivot = {1.0f, 0.0f};

        if (search)
        {
            ImGui::SetNextWindowPos(pos,ImGuiCond_Appearing, windowPivot);
            findObject(&search);
        }

        pos.y += findObjectHeight + widgetSpacing;

        if (groups)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);
            GroupMenu(&groups);
        }

        pos.y +=  GroupMenuHeight + widgetSpacing;

        if (togglePerformanceMode)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            performanceModeToggle(&togglePerformanceMode);
        }
        pos.y += PerformanceHeight + widgetSpacing;

        if (lightSource)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            setLightSourceMovementBehaviour(&lightSource);
        }

        pos.y += MovementLightSourceHeight + widgetSpacing;
        if (cameraMovement)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            setCameraMovementMode(&cameraMovement);
        }

        if (cameraBookmarks)
        {

            cameraBookmarkMenu(&cameraBookmarks);
        }
    }

    void Buttons::SideBarElement(const char* label, bool* state)
    {
        if (ImGui::Button(label))
        {
            *state = !*state;
        }
        ImGui::Spacing();
    }

    void Buttons::cameraBookmarkMenu(bool* visible)
    {

        auto bookmarks = saver->getGraph().getCameraBookmarks();

        ImGui::Begin("Bookmarks", visible,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::Button("Add Bookmark"))
        {
            addBookmarkWindow = true;
        }
        for (size_t i = 0; i < bookmarks.size(); ++i)
        {
            auto & bookmark = bookmarks[i];
            ImGui::CollapsingHeader(std::format("{}##{}",bookmark.getName(), i).c_str());
            ImGui::Text("Position: %.2f, %.2f, %.2f", bookmark.getCoordsVector().x, bookmark.getCoordsVector().y, bookmark.getCoordsVector().z);
            if (ImGui::Button(std::format("Load Bookmark##{}", i ).c_str()))
            {
                buttonController->loadCameraBookmark(bookmark);
            }
        }
        ImGui::End();

        if (addBookmarkWindow)
        {
            ImGui::OpenPopup("Add Bookmark",
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoCollapse
            );
            if (ImGui::BeginPopupModal("Add Bookmark",&addBookmarkWindow,
                ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoCollapse))
            {
                ImGui::InputText("Name", bookmarkName.data(), bookmarkName.size());
                if (ImGui::Button("Add"))
                {
                    buttonController->addCurrentPosAsBookmark(std::string(bookmarkName.data()));
                    bookmarkName = std::vector<char>(16);
                    addBookmarkWindow = false;

                }
                ImGui::EndPopup();
            }
        }



    }

    void Buttons::GroupMenu(bool* groupMenu)
    {


        activeGroups = &saver->getGraph().getGroups();

        ImGui::SetNextWindowSizeConstraints({260, 300},{MAXFLOAT, 300});
        ImGui::Begin("Groups", groupMenu,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse
            );

        if (ImGui::Button("Remove Group Highlights"))
        {
            buttonController->RemoveHighlights();
        }

        for (const auto& group : *activeGroups)
        {
            if (ImGui::CollapsingHeader(group.getName().c_str()))
            {
                ImGui::Text("Group ID: %d", group.getID());
                ImGui::SameLine();
                ImGui::ColorButton(std::format("Group Color##{}", group.getID()).c_str(),group.getVec4());
                ImGui::SameLine();

                randomizeColoring(group.getID());
                changeColoring(group.getID());

                ChangeTransparency(group.getID());


            }

        }

        ImGui::End();
    }


    void Buttons::ChangeTransparency(uint32_t groupID)
    {

        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        float& transparency = groupColors[groupID].w;

        if (transparency == 0.0f)
        {
            transparency = saver->getGraph().getGroupByID(groupID).getVec4().w;
        }

        if (ImGui::SliderFloat(std::format("##Transparency##{}", groupID).c_str(), &transparency, 0.0f, 1.0f))
        {
            buttonController->changeTransparency(groupID, transparency);
        }
    }


    void Buttons::changeColoring(uint32_t groupID)
    {


        if (groupID >= groupColors.size())
        {

            groupColors.resize(groupColors.size() * 2);

        }

        ImVec4& color = groupColors[groupID];

        if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0)
        {
            color = saver->getGraph().getGroupByID(groupID).getVec4();
        }


        ImGui::ColorEdit3(std::format("##Change Color Edit{}", groupID).c_str(), &color.x);
        if (ImGui::Button(std::format("Change Color##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, color);

        }

    }

    void Buttons::randomizeColoring(uint32_t groupID) const
    {

        if (ImGui::Button(std::format("Randomize Color ##{}", groupID).c_str()))
        {
            buttonController->randomizeColoring(groupID);
        }

    }

    void Buttons::performanceModeToggle(bool* toggle_mode)
    {
        const char* currentModeText;
        switch (performanceMode) {
            case PerformanceMode::QUALITY: currentModeText = "Quality"; break;
            case PerformanceMode::BALANCE: currentModeText = "Balance"; break;
            case PerformanceMode::PERFORMANCE: currentModeText = "Performance"; break;
        }

        ImGui::Begin("Performance Mode", toggle_mode,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::SliderInt("##ModeSlider", reinterpret_cast<int*>(&performanceMode),
            static_cast<int>(PerformanceMode::PERFORMANCE), static_cast<int>(PerformanceMode::QUALITY), currentModeText))
        {
            buttonController->togglePerformanceMode(performanceMode);
        }
        ImGui::End();
    }


    void Buttons::setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviorToggle)
    {

        ImGui::Begin("Light Source", lightSourceMovementBehaviorToggle,
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoCollapse
                );
        // Cast enum values to int
        const bool first = ImGui::RadioButton("Fixed Position",
            reinterpret_cast<int*>(&lightSourceMovementBehaviour),
            static_cast<int>(LightSourceMovementBehaviour::FIXED_POSITION));  // CAST TO INT

        const bool second = ImGui::RadioButton("Follow Camera",
            reinterpret_cast<int*>(&lightSourceMovementBehaviour),
            static_cast<int>(LightSourceMovementBehaviour::FOLLOW_CAMERA));  // CAST TO INT

        if (first || second)
        {
            buttonController->setLightSourceMovementBehaviour(lightSourceMovementBehaviour);
        }


        ImGui::End();


    }

    void Buttons::setCameraMovementMode(bool* cameraMovementMode) {
        ImGui::Begin("Camera Focus", cameraMovementMode,
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoCollapse
        );
        // Cast enum values to int
        const bool first = ImGui::RadioButton("Free Camera",
            reinterpret_cast<int*>(&cameraMode),
            static_cast<int>(CameraFocusMode::FREE));  // Or SELECTED_VERTEX if FREE doesn't exist

        const bool second = ImGui::RadioButton("Center of Mass",
            reinterpret_cast<int*>(&cameraMode),
            static_cast<int>(CameraFocusMode::CENTER_OF_MASS));  // CAST TO INT

        const bool third = ImGui::RadioButton("Origin",
            reinterpret_cast<int*>(&cameraMode),
            static_cast<int>(CameraFocusMode::ORIGIN));  // Add if you have this

        // Handle the radio button selection
        if (first || second || third) {
            buttonController->setCameraFocusMode(cameraMode);
        }
        ImGui::End();
    }

    void Buttons::findObject(bool* findObject)
    {

        ImGui::Begin("find Object", findObject,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        ImGui::BeginTabBar("##FindObjectTabBar");
        if (ImGui::BeginTabItem("Edge"))
        {
            findEdge();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Vertex"))
        {
            findVertex();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();


        ImGui::End();

    }

    void Buttons::findVertex()
    {
        ImGui::InputInt("##VertexID", &vertex);
        if (ImGui::Button("Find Vertex"))
        {
            buttonController->findVertex(vertex);
            vertex = 0;
        }
    }

    void Buttons::findEdge()
    {
        ImGui::InputInt2("##Vertex IDs:", edgeVertices);
        if (ImGui::Button("Find Edge"))
        {
            buttonController->findEdge(edgeVertices[0], edgeVertices[1]);
            edgeVertices[0] = 0;
            edgeVertices[1] = 0;
        }
    }

    void Buttons::highlightSubgraph()
    {
        if (ImGui::BeginMenu("Highlight Subgraph"))
        {
            highlightSubgraphBrowser.SetTitle("Highlight Subgraph");
            highlightSubgraphBrowser.Open();
            ImGui::EndMenu();
        }


        if (highlightSubgraphBrowser.HasSelected())
        {
            const std::filesystem::path result = highlightSubgraphBrowser.GetSelected();

            buttonController->highlightSubgraph(result);

            highlightSubgraphBrowser.ClearSelected();
        }
    }

    void Buttons::importGraph()
    {
        if (ImGui::BeginMenu("import Graph"))
        {
            importGraphBrowser.SetTitle("import Graph");
            importGraphBrowser.Open();
            ImGui::EndMenu();
        }

        if (importGraphBrowser.HasSelected())
        {
            const std::filesystem::path result = importGraphBrowser.GetSelected();

            buttonController->importGraph(result);

            importGraphBrowser.ClearSelected();
        }
    }

    void Buttons::importGroupConfiguration()
    {
        if (ImGui::BeginMenu("Import Group Config"))
        {
            importGroupConfigBrowser.SetTitle("Import Group Config");
            importGroupConfigBrowser.Open();
            ImGui::EndMenu();
        }


        if (importGroupConfigBrowser.HasSelected())
        {
            const std::filesystem::path result = importGroupConfigBrowser.GetSelected();

            buttonController->importGroupConfiguration(result);

            importGroupConfigBrowser.ClearSelected();
        }
    }

    void Buttons::exportGraph()
    {
        if (ImGui::BeginMenu("Export Graph"))
        {
            exportGraphBrowser.SetTitle("Choose Export Location");
            exportGraphBrowser.Open();

            ImGui::EndMenu();
        }


        if (exportGraphBrowser.HasSelected())
        {
            const std::filesystem::path result = exportGraphBrowser.GetDirectory();
            buttonController->exportGraph(result, ExportFormat::PNG); //TODO: Make Format selectable
            exportGraphBrowser.ClearSelected();
        }
    }
}
