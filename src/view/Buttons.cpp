//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"

#include <iostream>

#include "imgui/imgui.h"

#include "imgui-filebrowser/imfilebrowser.h"

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

    GroupMenu();

    SideBar();


    ImGui::Begin("Buttons", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


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

    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin("Test", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    performanceModeToggle();

    toggleLightSourceMovement();


    ImGui::End();
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

void Buttons::GroupMenu()
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

    ImVec2 pos;
    pos.x = framebufferWidth;
    pos.y = framebufferHeight / 2.0f;
    ImGui::SetNextWindowPos(pos, 0, {1.0f, 0.5f});

    ImVec2 size = {200,400};
    ImGui::SetNextWindowSize(size);

    ImGui::Begin("Groups", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
        );

    for (auto& group : groups)
    {
        if (ImGui::CollapsingHeader(group.name))
        {
            ImGui::Text("%d", group.groupID);
            ImGui::ColorButton("",group.color);

        }


    }

    ImGui::End();
}

void Buttons::performanceModeToggle()
{
    static const char* mode[] = {"Quality", "Balanced", "Performance"};
    ImGui::SliderInt("", &performanceMode, 0, 2, mode[performanceMode]);
}

void Buttons::randomizeColoring(int groupID)
{
}

void Buttons::changeColoring(int groupID)
{
}

void Buttons::toggleLightSourceMovement()
{
    ImGui::RadioButton("Light Source 1", &buttonController, 0);
    ImGui::RadioButton("Light Source 2", &buttonController, 1);
}

void Buttons::setLightSourceMovementBehaviour()
{
}

void Buttons::setCameraMovementMode()
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

void Buttons::SideBar()
{

    static bool search;
    static bool groups;
    static bool performanceMode;
    static bool lightMode;
    static bool capture;

    ImTextureID searchID = ImTextureID(1) ;

    ImGui::Begin("");
    ImGui::ImageButton();

}
