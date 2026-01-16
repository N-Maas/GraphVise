//
// Created by helen on 1/13/26.
//
#pragma once
#include <memory>
#include <vector>
#include "../controller/RendererObserver.hpp"

class RendererSubject {
private:
    std::vector<std::unique_ptr<RendererObserver>> observerList;

public:
    virtual int signIn(RendererObserver& observer);
    virtual int signOut(RendererObserver& observer);
    virtual void notify();
};
