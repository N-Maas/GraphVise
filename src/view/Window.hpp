//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP

#include "GUI.hpp"
#include "../controller/Exporting/CachingController.hpp"
#include "InputManager.hpp"
#include "controller/MovementController.hpp"

namespace graphvise
{
    class Window
    {
    public:
        Window();
        bool initWindow();

        void startApplicationLoop();

    private:

        std::shared_ptr<Renderer> renderer;
        static Graph WelcomeGraph();


        GLFWwindow* window = nullptr;
        int framebufferWidth = 0, framebufferHeight = 0;


        std::shared_ptr<ButtonController> buttonController;
        std::shared_ptr<GUI> gui;
        InputManager inputManager;

        struct Resolution
        {
            int width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP