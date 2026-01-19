//
// Created by helen on 1/13/26.
//
#pragma once
#include <memory>
#include <vector>
#include "../controller/RendererObserver.hpp"

class RendererSubject {
private:
    std::vector<std::shared_ptr<RendererObserver>> observerList;

public:
    RendererSubject() = default;
    virtual ~RendererSubject();

    virtual void signIn(std::shared_ptr<RendererObserver> observer);
    virtual void signOut(std::shared_ptr<RendererObserver> observer);
    virtual void notify();

    [[nodiscard]] const auto& getObserverList() const {return observerList;}
};
