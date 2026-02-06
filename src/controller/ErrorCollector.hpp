//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP
#define THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP
#include <memory>

#include "Error.hpp"
#include "ErrorCollectorSubject.hpp"

namespace graphvise {
    class ErrorCollector : ErrorCollectorSubject {
    public:
        static ErrorCollector& getInstance();
        void collectError(Error error);
        Error getCurrentError();
        void signIn(std::reference_wrapper<ErrorCollectorObserver> observer) override;
        void signOut(std::reference_wrapper<ErrorCollectorObserver> observer) override;
        ErrorCollector(const ErrorCollector&) = delete;
        ErrorCollector& operator=(const ErrorCollector&) = delete;
    private:
        static ErrorCollector instance;

        ErrorCollector() = default;
        std::optional<Error> currentError;

        std::vector<std::reference_wrapper<ErrorCollectorObserver>> observers;

        void notify() override;
    };
}

#endif //THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP