//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP


#include "GUI.hpp"
#include "controller/MovementController.hpp"

namespace graphvise
{
    class Window
    {
    public:
        Window();

        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        bool initWindow();

    private:
        void processEvents();
        //void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        GLFWwindow* window = nullptr;
        MovementController movementController;

        struct Resolution
        {
            int width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

        static double scrollYOffset;
    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP