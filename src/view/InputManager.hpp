//
// Created by Emile Brückner on 2/24/26.
//

#ifndef GRAPHVISE_INPUTMANAGER_HPP
#define GRAPHVISE_INPUTMANAGER_HPP

#include <functional>
#include <utility>

#include "controller/ButtonController.hpp"
#include "controller/MovementController.hpp"

namespace graphvise
{
    class InputManager
    {
    public:
        explicit InputManager(std::shared_ptr<ButtonController> buttonController) : buttonController(std::move(buttonController))
        {
        }

        void initInputManager(GLFWwindow* window);

        void processInput();
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    private:

        void processHotkeys();


        static double scrollYOffset;

        GLFWwindow* window = nullptr;
        MovementController movementController;
        std::shared_ptr<ButtonController> buttonController;


        struct Hotkey {
            std::vector<int> keys;
            std::function<void()> action;
            bool pressed = false;

        };

        std::vector<Hotkey> hotkeys = {
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_Q}, [this]() { buttonController->togglePerformanceMode(); }},
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L}, [this]() { buttonController->toggleLightSourceMovementBehaviour(); }},
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K}, [this]() { buttonController->toggleCameraFocusMode(); }}
            // {{},},
            // {{},},
            // {{},},
            // {{},}

        };

    };
}


#endif //GRAPHVISE_INPUTMANAGER_HPP