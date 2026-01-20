//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"

#include <iostream>

#include "imgui/imgui.h"

#include "imfilebrowser.h"
#include "../rendering/Group.hpp"

Buttons::Buttons(int buttonController)
{
    this->buttonController = buttonController;


    importGraphBrowser.SetTypeFilters(allowedFiles);
    importGroupConfigBrowser.SetTypeFilters(allowedFiles);
    highlightSubgraphBrowser.SetTypeFilters(allowedFiles);


}

void Buttons::loadButtonFrame()
{

    MenuBar();


    ImGui::ShowDemoWindow();

    GroupMenu();



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


    ImGui::Begin("Test", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    performanceModeToggle();

    toggleLightSourceMovement();



    ImGui::End();
}

void Buttons::MenuBar()
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
    static const std::vector<Group> exampleGroups = {
        {ImVec4(0.5,0.3,0.2,1.0), "Cool Group", 1},
        {ImVec4(0.2,0.3,0.5,1.0), "Not Cool Group", 2},
        {ImVec4(0.3,0.5,0.2,1.0), "Main Group", 3}
    };
    ImGui::Begin("Groups", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::BeginTabBar("Groups");

    for (auto group : exampleGroups)
    {
        if (ImGui::BeginTabItem(group.name.c_str()))
        {
            ImGui::Text("Group ID: %d", group.id);
            ImGui::ColorEdit4("Color", &group.color.x);
            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();
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