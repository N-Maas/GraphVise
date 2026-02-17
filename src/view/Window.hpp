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
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

        void handleMouseClick(int button, int action, int mods, double xpos, double ypos);

    private:
        void processEvents();

        GLFWwindow* window = nullptr;
        MovementController movementController;
        std::unique_ptr<GUI> gui;

        struct Resolution
        {
            uint64_t width, height;
        };

        Resolution HD = {1920, 1080};
        Resolution SD = {1280, 720};

        Resolution currentRes = HD;

        static double scrollYOffset;

        glm::vec3 m_clickedObjectPos{0.0f};
        bool m_hasClickedVertex = false;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    };
}
#endif //THESIS_FRAMEWORK_WINDOW_HPP