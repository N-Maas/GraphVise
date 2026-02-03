//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#define THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#include <condition_variable>
#include "RendererObserver.hpp"
#include "../rendering/RendererSubject.hpp"
#include "Parsing/ParserController.hpp"
#include "Structs/ThreadOperation.hpp"

namespace graphvise {
    class ThreadController : public RendererObserver {
    public:
        explicit ThreadController(RendererSubject& renderer);
        bool notifyBackgroundThread(ThreadOperation threadOperation);
        void update() override;

    private:
        std::thread backgroundThread;
        static ParserController parserController;
        static std::mutex mutex;
        static std::condition_variable conditionVariable;
        static std::optional<ThreadOperation> threadOperation;
        static bool operationDone;
        static void threadMain();
    };
}

#endif //THESIS_FRAMEWORK_THREADCONTROLLER_HPP