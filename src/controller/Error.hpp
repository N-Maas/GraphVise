//
// Created by yannik on 14.01.26.
//

#ifndef THESIS_FRAMEWORK_ERROR_HPP
#define THESIS_FRAMEWORK_ERROR_HPP
#include <cstdint>
#include <optional>
#include <string>

#include "Enums/ErrorType.hpp"

namespace graphvise {
    class Error {
    public:
        explicit Error(ErrorType type);
        explicit Error(ErrorType, std::string message);
        explicit Error(ErrorType, uint32_t line);
        explicit Error(ErrorType, std::string message, uint32_t line);
        [[nodiscard]] ErrorType getErrorType() const;
        [[nodiscard]] std::optional<int> getLine() const;
        [[nodiscard]] std::optional<std::string> getMessage() const;

    private:
        ErrorType errorType;
        std::optional<uint32_t> line;
        std::optional<std::string> message;

        void setMessage();
    };
}

#endif //THESIS_FRAMEWORK_ERROR_HPP