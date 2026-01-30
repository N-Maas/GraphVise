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

    private:

        int currentObjId = 0;
        Buttons buttons;
        bool errorAvailable = false;
        Error currentError = Error(ErrorType::NO_ERROR);

        void errorPopup();
        void currentObjInfo();
        void update() override;
    };
}

#endif //THESIS_FRAMEWORK_GUI_HPP
