//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"

#include <format>

#include "imgui/imgui.h"
#include "imgui-filebrowser/imfilebrowser.h"

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

    void SideBarElement(const char* label, bool* state)
    {
        if (ImGui::Button(label))
        {
            *state = !*state;
        }
        ImGui::Spacing();
    }

    void Buttons::SideBar()
    {

        static bool search;
        static bool groups;
        static bool togglePerformanceMode;
        static bool lightSource;
        static bool cameraMovement;


        ImVec2 pos;
        pos.x = framebufferWidth;
        pos.y = framebufferHeight / 2.0f;
        ImGui::SetNextWindowPos(pos, 0, {1.0f, 0.5f});


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


        ImGui::End();

        if (search)
        {
            findObject(&search);
        }

        if (groups)
        {
            GroupMenu(&groups);
        }

        if (togglePerformanceMode)
        {
            performanceModeToggle(&togglePerformanceMode);
        }
        if (lightSource)
        {
            setLightSourceMovementBehaviour(&lightSource);
        }
        if (cameraMovement)
        {
            setCameraMovementMode(&cameraMovement);
        }
    }

    void Buttons::GroupMenu(bool* groupMenu)
    {


        activeGroups = &saver->getGraph().getGroups();

        ImGui::SetNextWindowSizeConstraints({230, 0},{MAXFLOAT, 400});
        ImGui::Begin("Groups", groupMenu,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse
            );

        for (const auto& group : *activeGroups)
        {
            if (ImGui::CollapsingHeader(group.getName().c_str()))
            {
                ImGui::Text("Group ID: %d", group.getGroupID());
                ImGui::SameLine();
                ImGui::ColorButton(std::format("Group Color##{}", group.getGroupID()).c_str(),group.getGroupVec4());
                randomizeColoring(group.getGroupID());
                changeColoring(group.getGroupID());

            }

        }

        ImGui::End();
    }

    void Buttons::performanceModeToggle(bool* toggle_mode)
    {

        const char* modeText[] = {"High Performance", "High Resolution"};

        ImGui::Begin("Performance Mode", toggle_mode,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::SliderInt("##ModeSlider", reinterpret_cast<int*>(&performanceMode), HIGH_PERFORMANCE, HIGH_RESOLUTION, modeText[performanceMode]))
        {
            buttonController->togglePerformanceMode(performanceMode);
        }

        ImGui::End();
    }

    void Buttons::randomizeColoring(uint32_t groupID) const
    {

        if (ImGui::Button(std::format("Randomize Color ##{}", groupID).c_str()))
        {
            buttonController->randomizeColoring(groupID);
        }

    }

    void Buttons::changeColoring(uint32_t groupID) const
    {

        static auto groupColors = std::vector<ImVec4>(16);

        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        ImVec4& color = groupColors[groupID];

        if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0)
        {
             color = saver->getGraph().getGroupByID(groupID).getGroupVec4();
        }


        ImGui::ColorEdit4(std::format("##Change Color Edit{}", groupID).c_str(), &color.x);
        if (ImGui::Button(std::format("Change Color##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, color);

        }

    }


    void Buttons::setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviorToggle)
    {
        ImGui::Begin("Light Source", lightSourceMovementBehaviorToggle,
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoCollapse
                );
        const bool first =ImGui::RadioButton("Fixed Position", reinterpret_cast<int*>(&lightSourceMovementBehaviour), FIXED_POSITION);
        const bool second = ImGui::RadioButton("Follow Camera", reinterpret_cast<int*>(&lightSourceMovementBehaviour), FOLLOW_CAMERA);

        if (first || second)
        {
            buttonController->setLightSourceMovementBehaviour(lightSourceMovementBehaviour);
        }


        ImGui::End();


    }

    void Buttons::setCameraMovementMode(bool* cameraMovementMode)
    {

        ImGui::Begin("Camera Focus", cameraMovementMode,
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoCollapse
                );

        const bool first = ImGui::RadioButton("Free Camera", reinterpret_cast<int*>(&cameraMode), FREE);
        const bool second = ImGui::RadioButton("Center of Mass", reinterpret_cast<int*>(&cameraMode), CENTER_OF_MASS);

        if (first || second)
        {
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
