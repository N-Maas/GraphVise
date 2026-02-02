//
// Created by helen on 1/13/26.
//
#pragma once
#include <memory>
#include <vector>
#include "../controller/RendererObserver.hpp"

namespace graphvise {
    class RendererSubject {
    private:
        std::vector<std::reference_wrapper<RendererObserver>> observerList;

    public:
        RendererSubject() = default;
        virtual ~RendererSubject();

        virtual void signIn(std::reference_wrapper<RendererObserver> observer);
        virtual void signOut(std::reference_wrapper<RendererObserver> observer);
        virtual void notify();

        [[nodiscard]] const auto& getObserverList() const {return observerList;}
    };
}