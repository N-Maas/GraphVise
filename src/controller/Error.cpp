//
// Created by yannik on 14.01.26.
//

#include "Error.hpp"

namespace graphvise {
    Error::Error(ErrorType type) : errorType(type){
        setErrorMessage();
    }

    Error::Error(ErrorType type, uint32_t line) : errorType(type), line(line){
        setErrorMessage();
    }

    Error::Error(ErrorType type, std::string message) : errorType(type), message(message) {
        setErrorMessage();
    }

    Error::Error(ErrorType type, std::string message, uint32_t line) : errorType(type), line(line), message(message) {
        setErrorMessage();
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

    std::string Error::getErrorMessage() const {
        return errorMessage;
    }


    void Error::setErrorMessage()
    {
        switch (errorType)
        {
            case
                ErrorType::EDGE_DOES_NOT_EXIST:
                errorMessage = "The specified edge does not exist.";
                break;
            case ErrorType::EQUAL_VERTEX_IDS:
                errorMessage = "The provided vertex IDs are equal.";
                break;
            case ErrorType::VERTEX_ID_OUT_OF_BOUNDS:
                errorMessage = "One or more vertex IDs are out of bounds.";
                break;
            case ErrorType::TOO_MANY_VERTICES_IN_SUBGRAPH:
                errorMessage = "The subgraph contains too many vertices.";
                break;
            case ErrorType::INVALID_SUBGRAPH_EDGE:
                errorMessage = "The subgraph contains an invalid edge.";
                break;
            case ErrorType::DUPLICATE_EDGE:
                errorMessage = "The graph contains a duplicate edge.";
                break;
            case ErrorType::INVALID_VERTEX_COUNT:
                errorMessage = "The vertex count is invalid.";
                break;
            case ErrorType::INVALID_EDGE_COUNT:
                errorMessage = "The edge count is invalid.";
                break;
            case ErrorType::INVALID_FORMATTING:
                errorMessage = "The file has invalid formatting.";
                break;
            case ErrorType::FILE_NOT_FOUND:
                errorMessage = "The specified file was not found.";
                break;
            case ErrorType::NOT_A_VERTEX_ID:
                errorMessage = "The provided ID is not a valid vertex ID.";
                break;
            case ErrorType::INVALID_COLOR_VALUE:
                errorMessage = "The color value provided is invalid.";
                break;
            case ErrorType::INVALID_TRANSPARENCY_VALUE:
                errorMessage = "The transparency value provided is invalid.";
                break;
        case ErrorType::BACKGROUND_THREAD_ALREADY_BUSY:
                errorMessage = "The background thread is already busy.";
                break;
            case ErrorType::INVALID_EDGE_ID:
                errorMessage = "The provided edge ID is invalid.";
                break;
            case ErrorType::INVALID_VERTEX_ID:
                errorMessage = "The provided vertex ID is invalid.";
            break;
        case ErrorType::INVALID_IMPORT_FORMAT:
                errorMessage = "The provided import format is invalid.";
                break;
            default:
                errorMessage = "An unknown error occurred.";
                break;
        }




    }
}
