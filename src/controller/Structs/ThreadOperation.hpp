//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_THREADOPERATION_HPP
#define THESIS_FRAMEWORK_THREADOPERATION_HPP
#include <string>

#include "../Enums/ThreadOperationType.hpp"

#endif //THESIS_FRAMEWORK_THREADOPERATION_HPP

struct ThreadOperation {
    std::string filePath;
    ThreadOperationType requestedOperation;
};