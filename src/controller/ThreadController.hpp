//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#define THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#include <condition_variable>
#include <iostream>

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

        ~ThreadController() override {
            mutex.lock();
            terminateThread = true;
            mutex.unlock();
            conditionVariable.notify_all();

            if (backgroundThread.joinable()) {
                backgroundThread.join();
            }
        };
    private:
        std::thread backgroundThread;
        ParserController parserController;
        std::mutex mutex;
        std::condition_variable conditionVariable;
        std::optional<ThreadOperation> threadOperation;
        bool operationDone = false;
        bool terminateThread = false;
        void threadMain();
    };
}

#endif //THESIS_FRAMEWORK_THREADCONTROLLER_HPP