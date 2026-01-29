//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP


#include "GUI.hpp"

namespace graphvise
{
    class Window
    {
    public:
        Window();
        bool initWindow();
        void processEvents(GLFWwindow* m_window);

    private:
        struct Resolution
        {
            int width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

        GUI gui = GUI();
        bool mF5Pressed = false;

    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP