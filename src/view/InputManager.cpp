//
// Created by Emile Brückner on 2/24/26.
//

#include "InputManager.hpp"

#include <iostream>

#include "GUI.hpp"
#include "model/GraphSaver.hpp"

namespace graphvise
{
    double InputManager::scrollYOffset = 0;

    bool InputManager::mouseButtonPressed = false;
    int InputManager::button = 0;
    int InputManager::action = 0;
    int InputManager::mods = 0;

    void InputManager::initInputManager(GLFWwindow* window, GUI* gui,
                                        const std::shared_ptr<ButtonController>& buttonController)
    {
        glfwPollEvents();
        this->window = window;
        this->gui = gui;
        this->buttonController = buttonController;

        glfwSetWindowUserPointer(window, this);

        glfwSetScrollCallback(this->window, scrollCallback);
        glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
    }

    void InputManager::processInput()
    {
        // Take care of all GLFW events

        glfwPollEvents();


        const bool sprinting = (glfwGetKey(window, sprintKey) == GLFW_PRESS);


        if (!ImGui::GetIO().WantCaptureKeyboard)
        {
            //Moving Camera
            glm::vec3 direction(0, 0, 0);
            direction.z += (glfwGetKey(window, frontKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            direction.z -= (glfwGetKey(window, backKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            direction.x += (glfwGetKey(window, rightKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            direction.x -= (glfwGetKey(window, leftKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            direction.y += (glfwGetKey(window, upKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            direction.y -= (glfwGetKey(window, downKey) == GLFW_PRESS) ? 1.0f : 0.0f;
            movementController.moveCamera(direction, sprinting);
        }

        //Rotating Camera
        static float lastMousePosition[2];
        static double currentMousePositionDouble[2];
        glfwGetCursorPos(window, &currentMousePositionDouble[0], &currentMousePositionDouble[1]);
        float currentMousePositionFloat[2] = {
            static_cast<float>(currentMousePositionDouble[0]), static_cast<float>(currentMousePositionDouble[1])
        };


        static bool rotatingCamera = false;
        int rightMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
        if (!rotatingCamera && rightMouseState == GLFW_PRESS)
        {
            rotatingCamera = true;
            std::ranges::copy(currentMousePositionFloat, std::begin(lastMousePosition));
        }
        if (rotatingCamera)
        {
            float yawChange = currentMousePositionFloat[0] - lastMousePosition[0];
            float pitchChange = lastMousePosition[1] - currentMousePositionFloat[1];
            movementController.rotateCamera(pitchChange, yawChange);
            std::ranges::copy(currentMousePositionFloat, std::begin(lastMousePosition));
        }
        if (rotatingCamera && rightMouseState == GLFW_RELEASE)
        {
            rotatingCamera = false;
        }

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            movementController.zoom(static_cast<float>(-scrollYOffset), sprinting);
        }

        ImGui::GetIO().MouseWheel = static_cast<float>(scrollYOffset/4.0f);
        scrollYOffset = 0;

        handleMouseClick();

        processHotkeys();
    }

    void InputManager::processHotkeys()
    {
        for (auto& [keys, function, pressedInPrevFrame] : hotkeys)
        {
            bool allKeysPressed = true;

            for (const int key : keys)
            {
                if (glfwGetKey(window, key) != GLFW_PRESS)
                {
                    allKeysPressed = false;
                    break;
                }
            }
            if (allKeysPressed && !pressedInPrevFrame)
            {
                function();
                pressedInPrevFrame = true;
            }
            else if (!allKeysPressed)
            {
                pressedInPrevFrame = false;
            }
        }
    }

    void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        scrollYOffset = yoffset;
    }

    void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        mouseButtonPressed = true;

        InputManager::button = button;
        InputManager::action = action;
        InputManager::mods = mods;
    }

    void InputManager::handleMouseClick() const
    {
        if (!mouseButtonPressed || ImGui::GetIO().WantCaptureMouse)
        {
            return;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            const auto renderer = Renderer::getInstance();

            double xpos, ypos;

            glfwGetCursorPos(window, &xpos, &ypos);

            // Convert coordinates, needed for picking
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

            // Convert window coordinates to framebuffer coordinates
            int winWidth, winHeight;
            glfwGetWindowSize(window, &winWidth, &winHeight);

            double fbX = xpos * (static_cast<double>(fbWidth) / winWidth);
            double fbY = (winHeight - ypos) * (static_cast<double>(fbHeight) / winHeight);

            // Get the picked object (could be vertex, edge, or nothing)
            PickedObject picked = renderer->getObjectAt(fbX, fbY);

            if (picked.isVertex())
            {
                try
                {
                    gui->showVertexInfo(picked.id);
                }
                catch (const std::exception& e)
                {
                    std::cout << "Error getting vertex: " << e.what() << std::endl;
                }
            }
            else if (picked.isEdge())
            {
                try
                {
                    gui->showEdgeInfo(picked.id);
                }
                catch (const std::exception& e)
                {
                    // leave this empty so as not to show an error when no object is picked while clicking
                }
            }
        }
        mouseButtonPressed = false;
    }
}
