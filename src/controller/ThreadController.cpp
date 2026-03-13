//
// Created by yannik on 14.01.26.
//

#include "ThreadController.hpp"

#include <algorithm>
#include <iostream>
#include <thread>

#include "ErrorCollector.hpp"
#include "model/GraphSaver.hpp"
#include "rendering/Renderer.hpp"

namespace graphvise {

    ParserController ThreadController::parserController;
    ExporterController ThreadController::exporterController;
    std::mutex ThreadController::mutex;
    std::condition_variable ThreadController::conditionVariable;
    std::optional<ThreadOperation> ThreadController::threadOperation;
    std::optional<PNGExportData> ThreadController::pngExportData;
    bool ThreadController::operationDone = false;

    ThreadController::ThreadController(RendererSubject &renderer) : backgroundThread(&ThreadController::threadMain) {
        renderer.signIn(std::ref(*this));
        backgroundThread.detach();
    }

    void ThreadController::update() {
        if (operationDone) {
            std::optional<std::vector<GroupData>> groupData = parserController.getGroups();
            if (groupData.has_value()) {
                Graph& graph = GraphSaver::getInstance().getGraph();
                graph.deleteAllGroups();
                for (const auto&[name, color, vertices, edges] : groupData.value()) {
                    graph.addGroup(name, color, vertices, edges);
                }
            }

            std::optional<Graph> parsedGraph = parserController.getParsedGraph();
            if (parsedGraph.has_value()) {
                GraphSaver::getInstance().setGraph(parsedGraph.value());
                Renderer::getInstance()->m_camera().resetPosition();
            }

            std::optional<HighlightingData> highlightingSubgraph = parserController.getHighlightingSubgraph();
            if (highlightingSubgraph.has_value()) {
                Graph& graph = GraphSaver::getInstance().getGraph();

                graph.highlightByID(highlightingSubgraph.value().vertices, highlightingSubgraph.value().edges);
            }

            std::optional<Error> error = parserController.getError();
            if (error.has_value()) {
                ErrorCollector::getInstance().collectError(error.value());
            }

            parserController.resetData();
            threadOperation.reset();
            operationDone = false;
        }
    }

    bool ThreadController::notifyBackgroundThread(ThreadOperation threadOperation) {
        if (ThreadController::threadOperation.has_value()) {
            return false;
        }

        std::lock_guard lock(mutex);
        mutex.native_handle();
        ThreadController::threadOperation = threadOperation;

        if (threadOperation.requestedOperation == ThreadOperationType::EXPORT_PNG) {
            std::shared_ptr<Renderer> renderer = Renderer::getInstance();
            int width = renderer.get()->getFramebufferWidth();
            int height = renderer.get()->getFramebufferHeight();
            std::vector<unsigned char> pixels;
            pixels.resize(4 * width * height);

            //Read Pixels from Screen
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

            //Flip Picture upside down (made by AI)
            const int bytesPerPixel = 4;
            const int stride = width * bytesPerPixel;
            for (int y = 0; y < height / 2; ++y) {
                auto rowTop = pixels.begin() + (y * stride);
                auto rowBottom = pixels.begin() + ((height - 1 - y) * stride);
                std::swap_ranges(rowTop, rowTop + stride, rowBottom);
            }

            PNGExportData pngExportData(std::move(pixels), width, height);
            ThreadController::pngExportData = std::move(pngExportData);
        }

        conditionVariable.notify_all();
        return true;
    }

    bool ThreadController::backGroundThreadBusy()
    {
        return threadOperation.has_value();
    }

    void ThreadController::threadMain() {
        while (true) {
            //Wait for new operation
            std::unique_lock lock(mutex);
            conditionVariable.wait(lock, [] { return threadOperation.has_value() && !operationDone; });
            //Received new operation
            time_t timestamp;
            time(&timestamp);
            std::cout << "Received new operation at: " << ctime(&timestamp);
            ThreadOperation& operation = threadOperation.value();

            switch (operation.requestedOperation) {
                default: std::cout << "Invalid requested operation" << std::endl; operationDone = true; break;
                case ThreadOperationType::PARSE_TXT: {
                    parserController.parseFile(operation.filePath, ParseFormat::TXT);
                    operationDone = true;
                    break;
                }
                case ThreadOperationType::PARSE_GROUPS: {
                    parserController.parseFile(operation.filePath, ParseFormat::GROUP);
                    operationDone = true;
                    break;
                }
                case ThreadOperationType::PARSE_SUBGRAPH: {
                    parserController.parseFile(operation.filePath, ParseFormat::SUBGRAPH);
                    operationDone = true;
                    break;
                }
                case ThreadOperationType::PARSE_CNF: {
                    parserController.parseFile(operation.filePath, ParseFormat::CNF);
                    operationDone = true;
                    break;
                }
                case ThreadOperationType::EXPORT_PNG: {
                    exporterController.setPNGExportData(pngExportData.value());
                    exporterController.exportGraph(operation.filePath, ExportFormat::PNG);
                    operationDone = true;
                    break;
                }
            }
            time(&timestamp);
            std::cout << "Finished operation at: " << ctime(&timestamp);
        }
    }

    ThreadController::~ThreadController() {
        // Unregister from renderer
        auto rendererInstance = Renderer::getInstance();
        if (rendererInstance) {
            rendererInstance->signOut(std::ref(*this));
        }

        // Clean up any background threads
        if (backgroundThread.joinable()) {
            backgroundThread.join();
        }
    }

    RendererObserver::~RendererObserver() = default;
}
