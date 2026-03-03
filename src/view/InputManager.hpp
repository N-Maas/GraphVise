//
// Created by Emile Brückner on 2/24/26.
//

#ifndef GRAPHVISE_INPUTMANAGER_HPP
#define GRAPHVISE_INPUTMANAGER_HPP

#include <functional>
#include <utility>

#include "GUI.hpp"
#include "controller/ButtonController.hpp"
#include "controller/MovementController.hpp"

namespace graphvise
{
    class InputManager
    {
    public:
        void initInputManager(GLFWwindow* window, GUI* gui, const std::shared_ptr<ButtonController>& buttonController);

        void processInput();
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void handleMouseClick() const;

    private:
        void processHotkeys();


        // scroll callback
        static double scrollYOffset;

        // mouse callback
        static bool mouseButtonPressed;
        static int button;
        static int action;
        static int mods;


        GLFWwindow* window = nullptr;
        MovementController movementController;
        std::shared_ptr<ButtonController> buttonController;
        GUI* gui = nullptr;


        struct Hotkey
        {
            std::vector<int> keys;
            std::function<void()> action;
            bool pressed = false;
        };

        std::vector<Hotkey> hotkeys = {
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_Q}, [this]() { buttonController->togglePerformanceMode(); }},
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L}, [this]() { buttonController->toggleLightSourceMovementBehaviour(); }},
            {{GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K}, [this]() { buttonController->toggleCameraFocusMode(); }},
            {
                {GLFW_KEY_ESCAPE}, []()
                {
                    ImGui::CloseCurrentPopup();
                }
            },
            // {{},},
            // {{},},
            // {{},}

        };
    };
}


#endif //GRAPHVISE_INPUTMANAGER_HPP
