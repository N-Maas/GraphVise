//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_GUI_HPP
#define THESIS_FRAMEWORK_GUI_HPP

#include "Buttons.hpp"
#include "controller/ButtonController.hpp"

#include <GLFW/glfw3.h>

#include "ErrorCollectorObserver.hpp"

namespace graphvise
{
    class GUI : public ErrorCollectorObserver
    {
    public:
        GUI(ButtonController *controller);
        void initGUI(GLFWwindow* window);
        void loadFrame(int framebufferWidth, int framebufferHeight);
        void shutdownGUI();
        void setFps(double newFps);

        void showVertexInfo(uint32_t vertexId, const glm::vec2& screenPos = glm::vec2(-1));

        void vertexIDPopup();

    private:

        double fps = 0.0;
        Buttons buttons;
        bool errorAvailable;
        std::optional<Error> currentError = std::nullopt;

        // for vertex picking
        bool m_showVertexInfo;
        uint32_t m_selectedVertexId;
        glm::vec2 m_popupPosition;

        void errorPopup();
        void currentObjInfo();
        void update() override;
    };
}

#endif //THESIS_FRAMEWORK_GUI_HPP
