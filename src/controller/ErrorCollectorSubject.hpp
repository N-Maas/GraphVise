//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERRORCOLLECTORSUBJECT_HPP
#define THESIS_FRAMEWORK_ERRORCOLLECTORSUBJECT_HPP
#include <vector>

#include "../view/ErrorCollectorObserver.hpp"

namespace graphvise {
    class ErrorCollectorSubject {
    public:
        //Destruktor
        virtual ~ErrorCollectorSubject();

        virtual void signIn(std::reference_wrapper<ErrorCollectorObserver> observer) = 0;
        virtual void signOut(std::reference_wrapper<ErrorCollectorObserver> observer) = 0;

    private:
        std::vector<std::reference_wrapper<ErrorCollectorObserver>> observers;

        virtual void notify() = 0;
    };
}

#endif //THESIS_FRAMEWORK_ERRORCOLLECTORSUBJECT_HPP