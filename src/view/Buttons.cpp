//
// Created by Emile Brückner on 1/19/26.
//

#include "Buttons.hpp"

#include "imgui/imgui.h"
#include "imfilebrowser.h"

Buttons::Buttons(int buttonController)
{
    this->buttonController = buttonController;


    fileBrowser.SetTypeFilters({ ".txt"});

}

void Buttons::loadButonFrame()
{

    // ImGUI elements
    ImGui::Begin("CG Thesis Framework GUI");

    findVertex();
    ImGui::Text("Vertex ID: %d", vertex);

    findEdge();
    ImGui::Text("Edge ID: %d", edge);

    performanceModeToggle(&performanceMode);

    toggleLightSourceMovement();


    exportGraph();
    importGraph();
    fileBrowser.Display();


    ImGui::End();

}

void Buttons::performanceModeToggle(int *sliderVal)
{


    ImGui::SliderInt("", sliderVal, 0, 2, "Performance");


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
}

void Buttons::findEdge()
{
    ImGui::InputInt("Edge ID:", &edge);
    if(ImGui::Button("Find Edge"))
    {
        edge = 0;
    }
}

void Buttons::highlightSubgraph()
{

}

void Buttons::importGraph()
{


}

void Buttons::exportGraph()
{
    if(ImGui::Button("Export Graph"))
    {
        ImGuiFileBrowserFlags_SelectDirectory << 0;
        fileBrowser.SetTypeFilters({});
        fileBrowser.SetTitle("Choose Export Location");
        fileBrowser.Open();

    }


    if(fileBrowser.HasSelected())
    {
        ImGuiFileBrowserFlags_SelectDirectory << 1;
        fileBrowser.ClearSelected();
    }
}

void Buttons::importGroupConfiguration()
{

    if(ImGui::Button("Import Group Config"))
    {

        fileBrowser.SetTitle("Import Group Config");
        fileBrowser.Open();

    }


    if(fileBrowser.HasSelected())
    {

        fileBrowser.ClearSelected();
    }
}
