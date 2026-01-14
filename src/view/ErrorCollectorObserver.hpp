//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERRORCOLLECTOROBSERVER_HPP
#define THESIS_FRAMEWORK_ERRORCOLLECTOROBSERVER_HPP

#endif //THESIS_FRAMEWORK_ERRORCOLLECTOROBSERVER_HPP

class ErrorCollectorObserver {
    public:
    virtual ~ErrorCollectorObserver() = default;

    virtual void update() = 0;
};