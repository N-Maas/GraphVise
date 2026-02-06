//
// Created by yannik on 05.02.26.
//

#include "PNGExporter.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "lodepng/lodepng.h"

#define FILE_ENDING ".png"
#define OPTIONAL_FILE_SUFFIX(...) ("(" + __VA_ARGS__ + ")")

namespace graphvise {
    void PNGExporter::exportGraph(std::filesystem::path path) {


        std::string oldNameWithoutExtension = path.filename().replace_extension();

        //If file with same name already exists,
        if (checkFileExists(path)) {
            int fileNumber = 1;
            do {

                path = path.replace_filename(oldNameWithoutExtension + OPTIONAL_FILE_SUFFIX(std::to_string(fileNumber)) + path.extension().string());
                fileNumber++;
            } while (checkFileExists(path));
        }

        std::lock_guard lock(mutex);
        std::vector<unsigned char> pixels = pngExportData->pixels;
        int width = pngExportData->screenWidth;
        int height = pngExportData->screenHeight;
        lodepng::encode(path, pixels.data(), width, height);
        std::cout << "Exported to: " << path << std::endl;
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