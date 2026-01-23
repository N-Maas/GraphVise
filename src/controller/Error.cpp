//
// Created by yannik on 14.01.26.
//

#include "Error.hpp"

Error::Error(ErrorType type) : errorType(type){
}

Error::Error(ErrorType type, uint32_t line) : errorType(type), line(line){
}

Error::Error(ErrorType type, std::string message) : errorType(type), message(message) {
}

Error::Error(ErrorType type, std::string message, uint32_t line) : errorType(type), message(message), line(line) {
}

ErrorType Error::getErrorType() const {
    return errorType;
}

std::optional<int> Error::getLine() const {
    return line;
}

std::optional<std::string> Error::getMessage() const {
    return message;
}