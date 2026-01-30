//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"


#include "imgui/imgui.h"

#include "imgui-filebrowser/imfilebrowser.h"
#include "rendering/renderer.hpp"

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

        ImGui::ShowDemoWindow();

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

        static bool search;
        static bool groups;
        static bool performanceMode;
        static bool lightMode;


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

        if (ImGui::Button("Search"))
        {
            search = !search;
        }
        ImGui::Spacing();
        if (ImGui::Button("Groups"))
        {
            groups = !groups;
        }
        ImGui::Spacing();

        if (ImGui::Button("Performance Mode"))
        {
            performanceMode = !performanceMode;
        }
        ImGui::Spacing();
        if (ImGui::Button("Light Mode"))
        {
            lightMode = !lightMode;
        }
        ImGui::Spacing();

        ImGui::End();

        if (search)
        {
            findObject(&search);
        }

        if (groups)
        {
            GroupMenu(&groups);
        }

        if (performanceMode)
        {
            performanceModeToggle(&performanceMode);
        }
        if (lightMode)
        {
            toggleLightSourceMovement(&lightMode);
        }

    }

    void Buttons::GroupMenu(bool* groupMenu)
    {


        groups = &saver->getGraph().getGroups();

        ImGui::SetNextWindowSizeConstraints({170, 0},{MAXFLOAT, 400});
        ImGui::Begin("Groups", groupMenu,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse
            );

        for (auto group : *groups)
        {
            if (ImGui::CollapsingHeader(group.getName().c_str()))
            {
                ImGui::Text("%d", group.getGroupID());
                ImGui::SameLine();
                ImGui::ColorButton("",group.getGroupVec4());

            }

        }

        ImGui::End();
    }

    void Buttons::performanceModeToggle(bool* performanceMode)
    {

        const char* modeText[] = {"High Performance", "High Resolution"};

        ImGui::Begin("Performance Mode", performanceMode,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::SliderInt("##ModeSlider", (int*)&mode, 0, 1, modeText[mode]))
        {
            buttonController->togglePerformanceMode(mode);
        }

        ImGui::End();
    }

    void Buttons::randomizeColoring(int groupID)
    {
    }

    void Buttons::changeColoring(int groupID)
    {
    }

    void Buttons::toggleLightSourceMovement(bool* lightSourceMovement)
    {


        ImGui::Begin("Light Source", lightSourceMovement,
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoCollapse
        );

        ImGui::End();
    }

    void Buttons::setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviour)
    {



    }

    void Buttons::setCameraMovementMode(bool* cameraMovementMode)
    {

    }

    void Buttons::findObject(bool* findObject)
    {

        ImGui::Begin("find Object", findObject,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        ImGui::BeginTabBar("Hello");
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
        ImGui::InputInt("Vertex ID:", &vertex);
        if (ImGui::Button("Find Vertex"))
        {
            buttonController->findVertex(vertex);
            vertex = 0;
        }
    }

    void Buttons::findEdge()
    {
        ImGui::InputInt2("Vertex IDs:", edgeVertices);
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
            std::filesystem::path result = highlightSubgraphBrowser.GetSelected();

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
            std::filesystem::path result = importGraphBrowser.GetSelected();

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
            std::filesystem::path result = importGroupConfigBrowser.GetSelected();

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
            std::filesystem::path result = exportGraphBrowser.GetDirectory();
            buttonController->exportGraph(result, ExportFormat::PNG); //TODO: Make Format selectable
            exportGraphBrowser.ClearSelected();
        }
    }
}
