//
// Created by Emile Brückner on 1/19/26.
//

#ifndef THESIS_FRAMEWORK_GUI_HPP
#define THESIS_FRAMEWORK_GUI_HPP
#include <GLFW/glfw3.h>

#include "Buttons.hpp"

namespace graphvise
{
    class GUI
    {
    public:
        GUI();
        void initGUI(GLFWwindow* window);
        void loadFrame(int framebufferWidth, int framebufferHeight);
        void shutdownGUI();

    private:

        int currentObjId = -1;
        Buttons buttons = Buttons(0);

        void loadGUI();
        void errorPopup();
        void currentObjInfo();
    };
}
#endif //THESIS_FRAMEWORK_GUI_HPP