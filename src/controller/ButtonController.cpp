//
// Created by yannik on 14.01.26.
//

#include "ButtonController.hpp"

#include <random>

#define MAX_COLOR_VALUE 255

ButtonController::ButtonController(Camera& camera, Renderer& renderer) : camera(camera), renderer(renderer), threadController(renderer) {
}

void ButtonController::togglePerformanceMode(PerformanceMode mode) {
    //TODO: Implementieren, wenn es eine entsprechende Methode im Renderer gibt
}

void ButtonController::randomizeColoring(int groupID) {
    Group& group= GraphSaver::getGraphSaver().getGraph().getGroupByID(groupID);

    std::random_device random;
    std::mt19937 generator(random());
    std::uniform_real_distribution<> distribution(0, MAX_COLOR_VALUE);

    ImVec4 newColor = ImVec4(distribution(generator), distribution(generator), distribution(generator), MAX_COLOR_VALUE);
    group.setGroupVec4(newColor);
}
