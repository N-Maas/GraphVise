//
// Created by helen on 1/13/26.
//

#pragma once

namespace graphvise {
    class RendererObserver {
    public:
        virtual ~RendererObserver();
        virtual void update() = 0;
    };
}