//
// Created by yannik on 05.02.26.
//

#ifndef GRAPHVISE_EXPORTERCONTROLLER_HPP
#define GRAPHVISE_EXPORTERCONTROLLER_HPP

#include "PNGExporter.hpp"
#include "controller/Enums/ExportFormat.hpp"
namespace graphvise {
    class ExporterController {
    public:
        void exportGraph(std::filesystem::path path, ExportFormat format);
        void setPNGExportData(PNGExportData exportData);

    private:
        PNGExporter pngExporter;
    };
}
#endif //GRAPHVISE_EXPORTERCONTROLLER_HPP