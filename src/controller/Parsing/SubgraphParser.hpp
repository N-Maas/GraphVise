//
// Created by tim on 19.01.26. 
//

#ifndef THESIS_FRAMEWORK_CNFPARSER_HPP
#define THESIS_FRAMEWORK_CNFPARSER_HPP

#include "../Structs/GraphData.hpp"

#endif //THESIS_FRAMEWORK_CNFPARSER_HPP

class SubgraphParser {
    public:
        SubgraphParser() {
            // TODO: Implement constructor
        }
    private:
        GraphSaver graphSaver;

    //Destruktor
    virtual ~GraphParser() = default;

    virtual GraphData parseFile(std::string filePath) = 0;
};