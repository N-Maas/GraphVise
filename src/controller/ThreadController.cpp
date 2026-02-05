//
// Created by yannik on 14.01.26.
//

#include "ThreadController.hpp"

#include <iostream>
#include <thread>

#include "ErrorCollector.hpp"
#include "model/GraphSaver.hpp"
#include "rendering/renderer.hpp"

namespace graphvise {

    ParserController ThreadController::parserController;
    std::mutex ThreadController::mutex;
    std::condition_variable ThreadController::conditionVariable;
    std::optional<ThreadOperation> ThreadController::threadOperation;
    bool ThreadController::operationDone = false;

    ThreadController::ThreadController(RendererSubject &renderer) : backgroundThread(&ThreadController::threadMain) {
        std::reference_wrapper<RendererObserver> observer = std::ref(*this);
        renderer.signIn(observer);
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
                std::cout << "highlighting " << highlightingSubgraph.value().vertices.size() << " vertices" << std::endl;
                std::cout << "and " << highlightingSubgraph.value().edges.size() << " edges" << std::endl;

                for (auto edge : highlightingSubgraph.value().edges) {
                    std::cout << edge << std::endl;
                }

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
        ThreadController::threadOperation = threadOperation;
        conditionVariable.notify_all();
        std::cout << "Notified Background Thread" << std::endl;
        return true;
    }

   void ThreadController::threadMain() {
        while (true) {
            std::cout << "Waiting for new operation..." << std::endl;
            std::unique_lock lock(mutex);
            conditionVariable.wait(lock, [] { return threadOperation.has_value() && !operationDone; });
            std::cout << "Received new Data" << std::endl;

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
            }
        }
    }

    RendererObserver::~RendererObserver() = default;
}
