//
// Created by yannik on 05.02.26.
//

#include "ExporterController.hpp"

#include <iostream>
#include <utility>

namespace graphvise {
    void ExporterController::exportGraph(std::filesystem::path path, ExportFormat format) {
        if (format == ExportFormat::PNG) {
            pngExporter.exportGraph(std::move(path));
        } else {
            std::cout << "Export Format not implemented!" << std::endl;
        }
    }

    void ExporterController::setPNGExportData(PNGExportData exportData) {
        pngExporter.setPNGExportData(std::move(exportData));
    }
}
