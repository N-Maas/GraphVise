//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP
#define THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP
#include "Error.hpp"
#include "ErrorCollectorSubject.hpp"

class ErrorCollector : ErrorCollectorSubject {
    public:
        static ErrorCollector& getInstance();
        void collectError(Error error);
        Error getCurrentError();
        void signIn(ErrorCollectorObserver* observer) override;
        void signOut(ErrorCollectorObserver* observer) override;
        ~ErrorCollector() override = default;
    private:
        static ErrorCollector instance;

        ErrorCollector() = default;
        std::optional<Error> currentError;
        std::vector<ErrorCollectorObserver> observers;

        void notify() override;
};


#endif //THESIS_FRAMEWORK_ERRORCOLLECTOR_HPP