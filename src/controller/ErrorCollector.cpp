//
// Created by yannik on 14.01.26.
//

#include "ErrorCollector.hpp"

#include <utility>

#include "Rand.hpp"

namespace graphvise {
    ErrorCollector ErrorCollector::instance;

    ErrorCollector& ErrorCollector::getInstance() {
        return instance;
    }

    void ErrorCollector::collectError(Error error) {
        currentError = std::move(error);

        notify();
    }

    Error ErrorCollector::getCurrentError() {
        //Throws exception, if value is not set
        return currentError.value();
    }

    void ErrorCollector::notify() {
        for (ErrorCollectorObserver& observer : observers) {
            observer.update();
        }
    }

    void ErrorCollector::signIn(ErrorCollectorObserver *observer) {
        //TODO: implement
    }

    void ErrorCollector::signOut(ErrorCollectorObserver *observer) {
        //TODO: implement
    }

    ErrorCollectorSubject::~ErrorCollectorSubject() = default;
}