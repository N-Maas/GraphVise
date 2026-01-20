//
// Created by tim on 19.01.26.
//

#ifndef THESIS_FRAMEWORK_GRAPHPARSER_HPP
#define THESIS_FRAMEWORK_GRAPHPARSER_HPP
#include <string>

#include "../Structs/GroupData.hpp"
// TODO: Include graph saver from model once class exists
#include "../ThreadController.hpp"

#endif //THESIS_FRAMEWORK_GRAPHPARSER_HPP

class GroupParser {
    public:
    //Destruktor
    virtual ~GraphParser();

    virtual std::vector<GroupData> parseFile(std::string filePath) = 0;

    private:
        GraphSaver graphSaver;
        ThreadController threadController;
};