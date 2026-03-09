cmake_minimum_required(VERSION 3.16)
project(GraphVise VERSION 1.1 DESCRIPTION "GraphVise" LANGUAGES CXX)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory. Please use a build subdirectory.")
endif ()

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ======================================================================
# Fetch external dependencies
# ======================================================================
include(FetchContent)

# GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# wembed
FetchContent_Declare(
        wembed
        GIT_REPOSITORY https://github.com/Vraier/wembed
        GIT_TAG cpp-library-interface
        GIT_SHALLOW TRUE
)

# PlatformFolders
FetchContent_Declare(
        platform_folders
        GIT_REPOSITORY https://github.com/sago007/PlatformFolders.git
        GIT_TAG master
        GIT_SHALLOW TRUE
)

# Make all fetched content available
set(WEMBED_AS_LIBRARY ON)
FetchContent_MakeAvailable(googletest wembed platform_folders)

enable_testing()

# ======================================================================
# Find required packages
# ======================================================================
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenMP)

# Boost configuration
set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.83.0 REQUIRED COMPONENTS filesystem serialization system)

# ======================================================================
# Add external subdirectories
# ======================================================================
add_subdirectory(ext)  # glad and imgui

# ======================================================================
# Define library
# ======================================================================
add_library(my_lib
        src/model/CameraBookmark.cpp
        src/model/GraphSaver.cpp
        src/model/Edge.cpp
        src/model/Graph.cpp
        src/model/Group.cpp
        src/model/Vertex.cpp
        src/controller/Exporting/CachingController.cpp
)

target_include_directories(my_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ext
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad
        ${platform_folders_SOURCE_DIR}
        ext/imgui-filebrowser
)

target_link_libraries(my_lib PRIVATE
        glad
        Boost::serialization
        Boost::filesystem
        Boost::system
)

# ======================================================================
# Define source files
# ======================================================================
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
        src/view/InputManager.hpp
        src/controller/Enums/ImportFormat.hpp
        src/rendering/enums.hpp
        src/model/VertexTransparencyCompare.hpp
        src/model/EdgeTransparencyCompare.hpp
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
        src/view/InputManager.cpp
        src/controller/Exporting/CachingController.cpp
)

set(SHADERS
        shaders/simple.vert
        shaders/single_color.frag
        shaders/graph.vert
        shaders/graph_color.vert
)

set(SHADERS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

# ======================================================================
# Define main executable
# ======================================================================
add_executable(GraphVise ${SOURCES} ${HEADERS})

target_compile_definitions(GraphVise PRIVATE
        -DSHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shader/"
)

target_include_directories(GraphVise PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ext
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${platform_folders_SOURCE_DIR}
        ext/imgui-filebrowser
)

target_link_libraries(GraphVise PRIVATE
        imgui
        glad
        glfw
        OpenGL::GL
        wembedLib
        wembed_flags
        embeddingLib
        graphLib
        utilLib
        Boost::filesystem
        Boost::serialization
        platform_folders
        my_lib
)

if (OpenMP_CXX_FOUND)
    target_link_libraries(GraphVise PUBLIC OpenMP::OpenMP_CXX)
endif ()

# ======================================================================
# Tests (commented out for now)
# ======================================================================
# file(GLOB TEST_SOURCES tests/*.cpp)
# add_executable(my_tests ${TEST_SOURCES})
#
# target_include_directories(my_tests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/ext
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/ext/glad
#     ${platform_folders_SOURCE_DIR}
#     ext/imgui-filebrowser
# )
#
# target_link_libraries(my_tests PRIVATE
#     my_lib
#     gtest_main
#     glad
#     Boost::serialization
#     Boost::filesystem
#     Boost::system
#     platform_folders
# )
#
# include(GoogleTest)
# gtest_discover_tests(my_tests)

# ======================================================================
# Install
# ======================================================================
install(TARGETS GraphVise RUNTIME DESTINATION bin)