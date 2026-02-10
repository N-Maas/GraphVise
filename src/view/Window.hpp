//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP


#include <functional>

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
        void processHotkeys();


        struct Hotkey {
            std::vector<int> keys;
            std::function<void()> action;
            bool pressed = false;

        };

        std::vector<Hotkey> hotkeys = {
                {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_Q}, [this]() { buttonController.togglePerformanceMode(); }},
                {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L}, [this]() { buttonController.toggleLightSourceMovementBehaviour(); }},
                {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K}, [this]() { buttonController.toggleCameraFocusMode(); }}
            // {{},},
            // {{},},
            // {{},},
            // {{},}

        };



        GLFWwindow* window = nullptr;
        ButtonController buttonController;
        MovementController movementController;
        std::optional<GUI> gui = std::nullopt;

        struct Resolution
        {
            uint64_t width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

        static double scrollYOffset;
    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP