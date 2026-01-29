//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"


#include "imgui/imgui.h"

#include "imgui-filebrowser/imfilebrowser.h"

namespace graphvise
{
    Buttons::Buttons(int buttonController)
    {
        this->buttonController = buttonController;


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
        static bool capture;

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

        if (ImGui::Button("Capture"))
        {
            capture = !capture;
        }
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

        if (capture)
        {
            //capture functionality
        }
    }

    void Buttons::GroupMenu(bool* groupMenu)
    {
        struct MockGroup
        {
            const char* name;
            const int groupID;
            ImVec4 color;
        };
        std::vector<MockGroup> groups = {
            {"Cool Group", 3, ImVec4(1.0f, 0.5f, 0.3f, 0.0f)},
            {"Not Group", 2, ImVec4(1.0f, 0.8f, 0.2f, 0.5f)},
            {"Main Group", 1, ImVec4(0.1f, 0.2f, 0.9f, 1.0f)},

        };


        // auto size = ImVec2(300, 400);
        // ImGui::SetNextWindowSize(size);

        ImGui::SetNextWindowSizeConstraints({170, 0},{MAXFLOAT, 400});
        ImGui::Begin("Groups", groupMenu,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse
            );

        for (auto& [name, groupID, color] : groups)
        {
            if (ImGui::CollapsingHeader(name))
            {
                ImGui::Text("%d", groupID);
                ImGui::SameLine();
                ImGui::ColorButton("",color);

            }

        }

        ImGui::End();
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
            ImGui::Text("Edge ID: %d", edge);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Vertex"))
        {
            findVertex();
            ImGui::Text("Vertex ID: %d", vertex);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();

        ImGui::End();

    }

    void Buttons::performanceModeToggle(bool* performanceMode)
    {
        static const char* mode[] = {"Quality", "Balanced", "Performance"};
        ImGui::Begin("Performance Mode", performanceMode,
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoCollapse
        );

        ImGui::SliderInt("", &performanceModeSlider, 0, 2, mode[performanceModeSlider]);

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

        ImGui::RadioButton("Light Source 1", &buttonController, 0);
        ImGui::RadioButton("Light Source 2", &buttonController, 1);

        ImGui::End();
    }

    void Buttons::setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviour)
    {
    }

    void Buttons::setCameraMovementMode(bool* cameraMovementMode)
    {
    }

    void Buttons::findVertex()
    {
        ImGui::InputInt("Vertex ID:", &vertex);
        if (ImGui::Button("Find Vertex"))
        {
            vertex = 0;
        }
    }

    void Buttons::findEdge()
    {
        ImGui::InputInt("Edge ID:", &edge);
        if (ImGui::Button("Find Edge"))
        {
            edge = 0;
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
            exportGraphBrowser.ClearSelected();
        }
    }
}