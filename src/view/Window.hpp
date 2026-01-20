//
// Created by Emile Brückner on 1/13/26.
//

#ifndef THESIS_FRAMEWORK_WINDOW_HPP
#define THESIS_FRAMEWORK_WINDOW_HPP
#include "GUI.hpp"


class Window
{

private:
    GUI gui = GUI();

public:
    Window();

    int initWindow();
};


#endif //THESIS_FRAMEWORK_WINDOW_HPP