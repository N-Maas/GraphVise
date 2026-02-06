//
// Created by yannik on 14.01.26.
//

#include "ErrorCollector.hpp"

#include <memory>
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
        for (auto& observer : observers) {
            observer.get().update();
        }
    }

    void ErrorCollector::signIn(std::reference_wrapper<ErrorCollectorObserver> observer) {
        this->observers.push_back(std::move(observer));
    }

    void ErrorCollector::signOut(std::reference_wrapper<ErrorCollectorObserver> observer) {
        auto it = std::ranges::find_if(observers,
                                       [observer](const std::reference_wrapper<ErrorCollectorObserver> ref) {
                                           return &ref.get() == &observer.get();
                                       }
        );
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    ErrorCollectorSubject::~ErrorCollectorSubject() = default;
}