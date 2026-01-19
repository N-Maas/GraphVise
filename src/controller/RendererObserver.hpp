//
// Created by helen on 1/13/26.
//

#pragma once


class RendererObserver {
public:
    virtual ~RendererObserver() = default;
    virtual void update() = 0;
};