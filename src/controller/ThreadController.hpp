//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#define THESIS_FRAMEWORK_THREADCONTROLLER_HPP
#include "RendererObserver.hpp"
#include "../rendering/RendererSubject.hpp"
#include "Parsing/ParserController.hpp"
#include "Structs/ThreadOperation.hpp"

namespace graphvise {
    class ThreadController : RendererObserver {
    public:
        explicit ThreadController(RendererSubject& renderer);
        bool notifyBackgroundThread(ThreadOperation threadOperation);
        void update() override;
    private:
        ParserController parserController;
    };
}

#endif //THESIS_FRAMEWORK_THREADCONTROLLER_HPP