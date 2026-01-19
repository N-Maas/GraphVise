//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_BUTTONS_HPP
#define THESIS_FRAMEWORK_BUTTONS_HPP

//needed for FileBrowser DON'T DELETE
#include "imgui/imgui.h"
#include "imfilebrowser.h"


class Buttons
{
    public:
    Buttons(int buttonController);

    void loadButonFrame();

private:
    int buttonController = 0;
    int performanceMode = 0;

    int vertex = -1;
    int edge = -1;

    ImGui::FileBrowser fileBrowser = ImGui::FileBrowser();

    void performanceModeToggle(int* sliderVal);
    void randomizeColoring(int groupID);
    void changeColoring(int groupID);
    void toggleLightSourceMovement();
    void setLightSourceMovementBehaviour();
    void setCameraMovementMode();
    void findVertex();
    void findEdge();
    void highlightSubgraph();
    void importGraph();
    void exportGraph();
    void importGroupConfiguration();
    void openFileBrowser(const char* buttonName);
};


#endif //THESIS_FRAMEWORK_BUTTONS_HPP