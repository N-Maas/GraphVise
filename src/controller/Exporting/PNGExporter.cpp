//
// Created by yannik on 05.02.26.
//

#include "PNGExporter.hpp"

#include <fstream>
#include <iostream>

#include "lodepng/lodepng.h"

#define STANDARD_FILE_NAME "/GraphVise Export"
#define FILE_ENDING ".png"
#define OPTIONAL_FILE_SUFFIX(...) ("(" + __VA_ARGS__ + ")")

namespace graphvise {
    void PNGExporter::exportGraph(std::string path) {
        std::string fileName = path + STANDARD_FILE_NAME + FILE_ENDING;

        //If file with same name already exists,
        if (checkFileExists(fileName)) {
            int fileNumber = 1;
            do {
                fileName = path + STANDARD_FILE_NAME + OPTIONAL_FILE_SUFFIX(std::to_string(fileNumber)) + FILE_ENDING;
                fileNumber++;
            } while (checkFileExists(fileName));
        }

        std::lock_guard lock(mutex);
        std::vector<unsigned char> pixels = pngExportData->pixels;
        int width = pngExportData->screenWidth;
        int height = pngExportData->screenHeight;
        lodepng::encode(fileName, pixels.data(), width, height);
        std::cout << "Exported to: " << fileName << std::endl;
        pngExportData.reset();
    }

    void PNGExporter::setPNGExportData(PNGExportData exportData) {
        std::lock_guard lock(mutex);
        this->pngExportData = std::move(exportData);
    }

    bool PNGExporter::checkFileExists(std::string path) {
        std::ifstream fileStream(path, std::ios::in);
        return fileStream.good();
    }
}