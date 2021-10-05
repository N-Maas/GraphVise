#include "utils.hpp"
#include <signal.h>
#include <fstream>

using namespace utils;

void utils::log(const std::string& message)
{
    std::cout << "LOG: " << message << std::endl;
}


GLenum utils::glCheckError_(const char *file, int line, bool debugBreakOnError) {
    /**
     * Checks for Open GL errors. If debugBreakOnError is set to true, a debug break will be issued. When running the
     * program normally, this will stop the program. When running with a debugger, the debugging will stop at the call.
     *
     * @param file file name string to log
     * @param file file line to log
     * @return debugBreakOnError if a debug break should be called.
     */
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
#ifdef GL_STACK_OVERFLOW
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW"; break;
#endif
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        log(error + " | " + file + " (" + std::to_string(line) + ")");

        if(debugBreakOnError)
        {
            // More or less portable version to call a breakpoint during debugging.
            // If your comiler doesn't stop the program here during debugging, you can replace this line with
            // the corresponding __debugbreak, __builtin_debugtrap etc. call. Or place a debug break point here.
#ifdef SIGTRAP
            raise(SIGTRAP);
#else
            log(std::string("can't call a debug break point ") + " | " + __FILE__ + " (" + std::to_string(__LINE__) + ")");
#endif
        }
    }
    return errorCode;
}


GLuint utils::createShader(GLenum shaderType, const GLchar **shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    // Attach Vertex shader source to the vertex shader object
    glShaderSource(shader, 1, shaderSource, nullptr);
    // Compile the vertex shader into machine code
    glCompileShader(shader);

    // Check for compilation errors
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar errorLog[maxLength];
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        std::cerr << "Shader compilation error: " << std::string(errorLog) << std::endl;

        glDeleteShader(shader);
        shader = 0;
        return 0;
    }

    GL_CHECK_ERROR();
    return shader;
}


GLuint utils::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    /**
     * Creates and returns the name of a new shader program using the given vertex and fragment shader source.
     *
     * @param vertexShaderSource char array containing vertex shader source
     * @param fragmentShaderSource char array containing fragment shader source
     */
    // Create shader objects
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, &vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Wrap-up/link all the shaders together into the shader program
    glLinkProgram(shaderProgram);
    // Check for linking errors
    GLint params;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
    if(params != GL_TRUE) {
        GLchar programLog[512] = { 0 };
        GLint size = 0;
        glGetProgramInfoLog(shaderProgram, 512, &size, programLog);
        std::cerr << "Shader program linking error: " << std::string(programLog) << std::endl;

        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    GL_CHECK_ERROR();

    // Delete the now useless vertex and fragment shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    GL_CHECK_ERROR();

    return shaderProgram;
}

GLuint utils::createShaderProgramFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    /**
     * Creates and returns the name of a new shader program using the given vertex and fragment shader source.
     *
     * @param vertexShaderSource char array containing vertex shader source
     * @param fragmentShaderSource char array containing fragment shader source
     */
    // read source from files
    std::ifstream vertexFileStream(vertexShaderPath);
    std::string vertexSource((std::istreambuf_iterator<char>(vertexFileStream)),std::istreambuf_iterator<char>());
    std::ifstream fragmentFileStream(fragmentShaderPath);
    std::string fragmentSource((std::istreambuf_iterator<char>(fragmentFileStream)),std::istreambuf_iterator<char>());

    // build shader
    return createShaderProgram(vertexSource.c_str(), fragmentSource.c_str());
}

