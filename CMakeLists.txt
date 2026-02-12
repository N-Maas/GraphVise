cmake_minimum_required(VERSION 3.16)
project(GraphVise VERSION 1.1 DESCRIPTION "GraphVise" LANGUAGES CXX)

# Require out-of-source builds (this is good practice)
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Use C++20 for wembed compatibility
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# source files -----------------------------------------------------------
# All files that will be compiled have to be listed here. The shaders variable is unused.

# GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(my_tests ${TEST_SOURCES})

add_library(my_lib
        src/model/CameraBookmark.cpp
        src/model/GraphSaver.cpp
        src/model/Edge.cpp
        src/model/Graph.cpp
        src/model/Group.cpp
        src/model/Vertex.cpp
)

target_include_directories(my_lib PUBLIC src ext)
target_link_libraries(my_tests PRIVATE my_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(my_tests)

set(HEADERS
        src/rendering/camera.hpp
        src/rendering/renderer.hpp
        src/rendering/enums.hpp
        src/utils.hpp
        src/rendering/RendererSubject.hpp
        src/controller/RendererObserver.hpp
        src/controller/ButtonController.hpp
        src/controller/ThreadController.hpp
        src/controller/Parsing/ParserController.hpp
        src/controller/Parsing/TXTParser.hpp
        src/controller/Parsing/CNFParser.hpp
        src/controller/Parsing/GraphParser.hpp
        src/controller/Parsing/TXTParser.hpp
        src/controller/Parsing/WEmbedController.hpp
        src/controller/MovementController.hpp
        src/controller/Error.hpp
        src/controller/ErrorCollector.hpp
        src/controller/ErrorCollectorSubject.hpp
        src/controller/Enums/ParseFormat.hpp
        src/controller/Enums/ErrorType.hpp
        src/controller/Enums/ThreadOperationType.hpp
        src/controller/Enums/ExportFormat.hpp
        src/controller/Structs/GroupData.hpp
        src/controller/Structs/GraphData.hpp
        src/controller/Structs/ThreadOperation.hpp
        src/view/ErrorCollectorObserver.hpp
        ext/imgui-filebrowser/imfilebrowser.h
        src/view/Window.hpp
        src/model/GraphSaver.hpp
        src/model/Graph.hpp
        src/model/Group.hpp
        src/model/CameraBookmark.hpp
        src/model/Vertex.hpp
        src/model/Edge.hpp
        src/controller/Parsing/GroupParser.hpp
        src/controller/Structs/HighlightingData.hpp
        ext/lodepng/lodepng.h
        src/controller/Exporting/ExporterController.hpp
        src/controller/Exporting/PNGExporter.hpp
        src/controller/Structs/PNGExportData.hpp
)

set(SOURCES
        src/rendering/camera.cpp
        src/main.cpp
        src/rendering/renderer.cpp
        src/utils.cpp
        src/view/Window.cpp
        src/controller/ButtonController.cpp
        src/controller/ThreadController.cpp
        src/controller/Parsing/ParserController.cpp
        src/controller/Parsing/TXTParser.cpp
        src/controller/Parsing/CNFParser.cpp
        src/controller/MovementController.cpp
        src/controller/Error.cpp
        src/controller/ErrorCollector.cpp
        src/model/GraphSaver.cpp
        src/model/Graph.cpp
        src/model/Group.cpp
        src/model/CameraBookmark.cpp
        src/model/Vertex.cpp
        src/model/Edge.cpp
        src/view/GUI.cpp
        src/view/Buttons.cpp
        src/controller/Parsing/WEmbedController.cpp
        src/controller/Parsing/GroupParser.cpp
        ext/lodepng/lodepng.cpp
        src/controller/Exporting/ExporterController.cpp
        src/controller/Exporting/PNGExporter.cpp
        src/controller/Enums/ImportFormat.hpp
        src/rendering/enums.hpp
)

set(SHADERS
    shaders/simple.vert
    shaders/single_color.frag
    shaders/graph.vert
    shaders/graph_color.vert
)

set(SHADERS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

# define target ---------------------------------------------------------- 

# add executable
# this is the build target that we create in the end and that we link all libraries to
add_executable(GraphVise ${SOURCES} ${HEADERS})

# set defines that can be used in the project code
# SHADERS_PATH points to the shader files directory
target_compile_definitions(GraphVise PRIVATE -DSHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shader/")

# external dependencies and libraries ------------------------------------

# find required packages
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# find optional packages
find_package(OpenMP)
# fetch wembed
include(FetchContent)

FetchContent_Declare(
        wembed
        GIT_REPOSITORY https://github.com/Vraier/wembed
        GIT_TAG        cpp-library-interface
        GIT_SHALLOW TRUE
)

set(WEMBED_AS_LIBRARY ON)
FetchContent_MakeAvailable(wembed)

# add external libraries that are locally built: glad and imgui
add_subdirectory(ext)

target_include_directories(GraphVise PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ext
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ext/imgui-filebrowser
        )

# link required libraries
target_link_libraries(GraphVise PRIVATE
    imgui
    glad
    glfw
    OpenGL::GL
    wembedLib # main wembed library
    wembed_flags # compiler flags
    embeddingLib # will be available from wembed
    graphLib # will be available from wembed
    utilLib # will be available from wembed
)
# link optional libraries
if(OpenMP_CXX_FOUND)
    target_link_libraries(GraphVise PUBLIC OpenMP::OpenMP_CXX)
endif()

# Install target (optional but good practice)
# install(TARGETS GraphVise RUNTIME DESTINATION bin)
