//
// Created by yannik on 14.01.26.
//

#include "Error.hpp"

namespace graphvise {
    Error::Error(ErrorType type) : errorType(type){
        setMessage();
    }

    Error::Error(ErrorType type, uint32_t line) : errorType(type), line(line){
        setMessage();
    }

    Error::Error(ErrorType type, std::string message) : errorType(type), message(message) {
        setMessage();
    }

    Error::Error(ErrorType type, std::string message, uint32_t line) : errorType(type), message(message), line(line) {
        setMessage();
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

    void Error::setMessage()
    {
        switch (errorType)
        {
            case
                ErrorType::EDGE_DOES_NOT_EXIST:
                message = "The specified edge does not exist.";
                break;
            case ErrorType::EQUAL_VERTEX_IDS:
                message = "The provided vertex IDs are equal.";
                break;
            case ErrorType::VERTEX_ID_OUT_OF_BOUNDS:
                message = "One or more vertex IDs are out of bounds.";
                break;
            case ErrorType::TOO_MANY_VERTICES_IN_SUBGRAPH:
                message = "The subgraph contains too many vertices.";
                break;
            case ErrorType::INVALID_SUBGRAPH_EDGE:
                message = "The subgraph contains an invalid edge.";
                break;
            case ErrorType::DUPLICATE_EDGE:
                message = "The graph contains a duplicate edge.";
                break;
            case ErrorType::INVALID_VERTEX_COUNT:
                message = "The vertex count is invalid.";
                break;
            case ErrorType::INVALID_EDGE_COUNT:
                message = "The edge count is invalid.";
                break;
            case ErrorType::INVALID_FORMATTING:
                message = "The file has invalid formatting.";
                break;
            case ErrorType::FILE_NOT_FOUND:
                message = "The specified file was not found.";
                break;
            case ErrorType::NOT_A_VERTEX_ID:
                message = "The provided ID is not a valid vertex ID.";
                break;
            case ErrorType::INVALID_COLOR_VALUE:
                message = "The color value provided is invalid.";
                break;
            case ErrorType::INVALID_TRANSPARENCY_VALUE:
                message = "The transparency value provided is invalid.";
                break;
        case ErrorType::BACKGROUND_THREAD_ALREADY_BUSY:
                message = "The background thread is already busy.";
                break;
            case ErrorType::INVALID_EDGE_ID:
                message = "The provided edge ID is invalid.";
                break;
            case ErrorType::INVALID_VERTEX_ID:
                message = "The provided vertex ID is invalid.";
            break;
            default:
                message = "An unknown error occurred.";
                break;
        }




    }
}
