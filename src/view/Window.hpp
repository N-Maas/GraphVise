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


        bool initWindow();
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    private:
        void processEvents();
        //void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        GLFWwindow* window = nullptr;
        MovementController movementController;

        struct Resolution
        {
            uint64_t width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

        static double scrollYOffset;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP