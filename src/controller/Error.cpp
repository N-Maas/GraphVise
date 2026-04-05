//
// Created by yannik on 14.01.26.
//

#include "Error.hpp"

#include <format>

namespace graphvise
{
    Error::Error(ErrorType type) : errorType(type)
    {
        setErrorMessage();
    }

    Error::Error(ErrorType type, uint32_t line) : errorType(type), line(line)
    {
        setErrorMessage();
    }

    Error::Error(ErrorType type, std::string message) : errorType(type), message(message)
    {
        setErrorMessage();
    }

    Error::Error(ErrorType type, std::string message, uint32_t line) : errorType(type), line(line), message(message)
    {
        setErrorMessage();
    }

    ErrorType Error::getErrorType() const
    {
        return errorType;
    }

    std::optional<int> Error::getLine() const
    {
        return line;
    }

    std::optional<std::string> Error::getMessage() const
    {
        return message;
    }

    std::string Error::getErrorMessage() const
    {
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
            if (!getLine().has_value())
            {
                errorMessage = "The provided vertex IDs are equal.";
            }
            else
            {
                errorMessage = std::format("A vertex IDs in line {} \"{}\" are equal.", getLine().value(),
                                           getMessage().value());
            }
            break;
        case ErrorType::VERTEX_ID_OUT_OF_BOUNDS:
            errorMessage = std::format("The ID \"{}\" is out of bounds.", getMessage().value());
            break;
        case ErrorType::TOO_MANY_VERTICES_IN_SUBGRAPH:
            errorMessage = "The provided Subgraph contains more vertices than the currently loaded Graph.";
            break;
        case ErrorType::INVALID_SUBGRAPH_EDGE:
            errorMessage = std::format("The provided edge \"{}\" is not part of the currently loaded Graph.",
                                       getMessage().value());
            break;
        case ErrorType::DUPLICATE_EDGE:
            errorMessage = "The graph contains a duplicate edge.";
            errorMessage = std::format("The edge \"{}\" in line {} appears multiple times.", getMessage().value(),
                                       getLine().value());
            break;
        case ErrorType::INVALID_VERTEX_COUNT:
            errorMessage = std::format("Invalid number of vertices provided in line {} \"{}\"", getLine().value(),
                                       getMessage().value());
            break;
        case ErrorType::INVALID_EDGE_COUNT:
            if (getMessage().has_value() && getLine().has_value())
            {
                errorMessage = std::format("Invalid number of edges provided in line {} \"{}\" ", getLine().value(),
                                           getMessage().value());
            }
            else
            {
                errorMessage = "The number of edges does not equal the provided value.";
            }
            break;
        case ErrorType::INVALID_FORMATTING:
            errorMessage = std::format("Line {} \"{}\" has invalid formatting.", getLine().value(), message.value());
            break;
        case ErrorType::FILE_NOT_FOUND:
            errorMessage = std::format("The specified file at {} could not be found.", message.value());
            break;
        case ErrorType::NOT_A_VERTEX_ID:
            if (message.has_value() && line.has_value())
            {
                errorMessage = std::format("The provided ID in line {} \"{}\" is out of bounds.", line.value(),
                                           message.value());
            }
            else
            {
                errorMessage = "The provided ID is not a valid vertex ID.";
            }
            break;
        case ErrorType::INVALID_COLOR_VALUE:
            errorMessage = std::format("The color value provided in line {} \"{}\" is invalid.", getLine().value(),
                                       getMessage().value());
            break;
        case ErrorType::INVALID_TRANSPARENCY_VALUE:
            errorMessage = std::format("The transparency value provided in line {} \"{}\" is invalid.",
                                       getLine().value(), getMessage().value());
            break;
        case ErrorType::BACKGROUND_THREAD_ALREADY_BUSY:
            errorMessage = "An operation is already running.";
            break;
        case ErrorType::INVALID_EDGE_ID:
            errorMessage = std::format("The provided edge ID in line {} \"{}\" is invalid.", getLine().value(),
                                       getMessage().value());
            break;
        case ErrorType::INVALID_VERTEX_ID:
            errorMessage = std::format("The provided vertex ID in line {} \"{}\" is invalid", getLine().value(),
                                       getMessage().value());
            break;
        case ErrorType::INVALID_IMPORT_FORMAT:
            errorMessage = "The provided import format is invalid.";
            break;
        case ErrorType::FILE_CORRUPTED:
            errorMessage = std::format("The file is corrupted. Filename: {}", getMessage().value());
            break;
        default:
            errorMessage = "An unknown error occurred.";
            break;
        }
    }
}
