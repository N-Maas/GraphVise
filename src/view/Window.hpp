//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP
#include "GUI.hpp"


class Window
{
public:
    Window();
    bool initWindow();
    void processEvents(GLFWwindow* m_window);

private:
    GUI gui = GUI();
    bool mF5Pressed;
    Renderer renderer;
};


#endif //THESIS_FRAMEWORK_WINDOW_HPP