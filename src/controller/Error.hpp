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
        explicit Error(ErrorType type);
        explicit Error(ErrorType, std::string message);
        explicit Error(ErrorType, int line);
        explicit Error(ErrorType, std::string message, int line);
        [[nodiscard]] ErrorType getErrorType() const;
        [[nodiscard]] std::optional<int> getLine() const;
        [[nodiscard]] std::optional<std::string> getMessage() const;

    private:
        ErrorType errorType;
        std::optional<int> line;
        std::optional<std::string> message;
};


#endif //THESIS_FRAMEWORK_ERROR_HPP