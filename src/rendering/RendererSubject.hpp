//
// Created by helen on 1/13/26.
//
#pragma once
#include <memory>
#include <vector>
#include "../controller/RendererObserver.hpp"

class RendererSubject {
public:
    std::vector<std::unique_ptr<RendererObserver>> observerList;
    virtual void signIn(RendererObserver& observer);
    virtual void signOut(RendererObserver& observer);
    virtual void notify();
};
