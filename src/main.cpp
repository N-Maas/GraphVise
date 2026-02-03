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

int main()
{
    //TODO: Remove when threadController is ready
    time_t timestamp;
    time(&timestamp);
    std::cout << std::endl;
    std::cout << "Starting Loading Graph at current time: " << ctime(&timestamp) << std::endl;
    std::cout << std::endl;

    graphvise::ParserController pc; //Testing TXT parser
    pc.parseFile("../Testgraph.txt", graphvise::ParseFormat::TXT);
    graphvise::GraphSaver::getInstance().setGraph(pc.getParsedGraph().value());

    time(&timestamp);
    std::cout << std::endl;
    std::cout << "Finished Loading Graph at current time: " << ctime(&timestamp) << std::endl;
    std::cout << std::endl;


    graphvise::Window window = graphvise::Window();


    if (!window.initWindow())
    {
    std::exit(1);
    }

    std::exit(0);

}
