//
// Created by yannik on 05.02.26.
//

#ifndef GRAPHVISE_PNGEXPORTER_HPP
#define GRAPHVISE_PNGEXPORTER_HPP
#include <condition_variable>
#include <optional>
#include <string>
#include <vector>

#include "controller/Structs/PNGExportData.hpp"

namespace graphvise {
    class PNGExporter {
    public:
        void exportGraph(std::string path);
        void setPNGExportData(PNGExportData exportData);

    private:
        std::mutex mutex;
        std::optional<PNGExportData> pngExportData;

        bool checkFileExists(std::string path);
    };
} // graphvise

#endif //GRAPHVISE_PNGEXPORTER_HPP