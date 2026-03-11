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
    class ButtonController;

    class GUI : public ErrorCollectorObserver
    {
    public:
        explicit GUI(const std::shared_ptr<ButtonController>& controller);
        void initGUI(GLFWwindow* window);
        void loadFrame(int framebufferWidth, int framebufferHeight);
        void shutdownGUI();
        void setFps(double newFps);

        void showVertexInfo(uint32_t vertexId);
        void showEdgeInfo(uint32_t edgeId);

        void clearSelection() {
            m_showObjectInfo = false;
            m_selectedVertexId = UINT32_MAX;
            m_selectedEdgeId = UINT32_MAX;
            m_objectType = ObjType::NONE;
        }

        GUI* get() { return this; }
    private:
        double fps = 0.0;
        Buttons buttons;
        bool errorAvailable;
        std::optional<Error> currentError = std::nullopt;

        std::shared_ptr<ButtonController> buttonController;
        // for vertex picking
        bool m_showObjectInfo = false;
        uint32_t m_selectedVertexId = UINT32_MAX;
        uint32_t m_selectedEdgeId = UINT32_MAX;
        glm::vec2 m_popupPosition;
        ObjType m_objectType = ObjType::NONE;  // 0=none, 1=vertex, 2=edge
        PickedObject m_pickedObject;  // Store the picked object

        void errorPopup();
        void currentObjInfo();
        void update() override;
    };
}

#endif //THESIS_FRAMEWORK_GUI_HPP
