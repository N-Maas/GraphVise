//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#define THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#include <condition_variable>
#include "RendererObserver.hpp"
#include "../rendering/RendererSubject.hpp"
#include "Exporting/ExporterController.hpp"
#include "Parsing/ParserController.hpp"
#include "Structs/ThreadOperation.hpp"

namespace graphvise {
    class ThreadController : public RendererObserver {
    public:
        explicit ThreadController(RendererSubject& renderer);
        ~ThreadController() override;
        bool notifyBackgroundThread(ThreadOperation threadOperation);
        static bool backGroundThreadBusy();
        void update() override;

    private:
        std::thread backgroundThread;
        static ParserController parserController;
        static ExporterController exporterController;
        static std::mutex mutex;
        static std::condition_variable conditionVariable;
        static std::optional<ThreadOperation> threadOperation;
        static std::optional<PNGExportData> pngExportData;
        static bool operationDone;
        static void threadMain();
    };
}

#endif //THESIS_FRAMEWORK_THREADCONTROLLER_HPP