//  Copyright (C) 2025, Max Piochowiak, Karlsruhe Institute of Technology
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <iostream>

#include "controller/Parsing/ParserController.hpp"
#include "model/GraphSaver.hpp"
#include "view/Window.hpp"

#include <filesystem>

int main()
{
    std::cout << "Program starting..." << std::endl;
    graphvise::Window window = graphvise::Window();

    graphvise::ParserController pc;

    if (!window.initWindow())
    {
        std::_Exit(1);
    }

    window.startApplicationLoop();

    std::_Exit(0);
}