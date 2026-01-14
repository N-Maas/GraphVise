//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERROR_HPP
#define THESIS_FRAMEWORK_ERROR_HPP
#include <optional>
#include <string>

#include "Enums/ErrorType.hpp"


class Error {
    public:
        explicit Error(ErrorType type, std::optional<int> line, std::optional<std::string> message);
        ErrorType getErrorType();
        std::optional<int> getLine();
        std::optional<std::string> getMessage();

    private:
        ErrorType errorType;
        std::optional<int> line;
        std::optional<std::string> message;
};


#endif //THESIS_FRAMEWORK_ERROR_HPP