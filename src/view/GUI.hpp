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
        GUI(const std::shared_ptr<ButtonController>& controller);
        void initGUI(GLFWwindow* window);
        void loadFrame(int framebufferWidth, int framebufferHeight);
        void shutdownGUI();
        void setFps(double newFps);

        void showVertexInfo(uint32_t vertexId);
        void showEdgeInfo(uint32_t edgeId);

    private:
        double fps = 0.0;
        Buttons buttons;
        bool errorAvailable;
        std::optional<Error> currentError = std::nullopt;

        // for vertex picking
        bool m_showObjectInfo;
        uint32_t m_selectedVertexId;
        uint32_t m_selectedEdgeId;
        glm::vec2 m_popupPosition;
        uint32_t m_objectType;
        PickedObject m_pickedObject;  // Store the picked object

        void errorPopup();
        static void currentObjInfo();
        void update() override;
    };
}

#endif //THESIS_FRAMEWORK_GUI_HPP
