cmake_minimum_required(VERSION 3.16)
project(thesis-framework VERSION 1.0 DESCRIPTION "CG Thesis Framework" LANGUAGES CXX)

# Require out-of-source builds (this is good practice)
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Use C++17 in all targets
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# source files -----------------------------------------------------------
# All files that will be compiled have to be listed here. The shaders variable is unused.

set(HEADERS
    src/camera.hpp
    src/renderer.hpp
    src/utils.hpp
)

set(SOURCES
    src/camera.cpp
    src/main.cpp
    src/renderer.cpp
    src/utils.cpp
)

set(SHADERS
    shaders/simple.vert
    shaders/single_color.frag
)

# define target ---------------------------------------------------------- 

# add executable
# this is the build target that we create in the end and that we link all libraries to
add_executable(thesis-framework  ${HEADERS} ${SOURCES} ${IMGUI})

# set defines that can be used in the project code
# SHADERS_PATH points to the shader files directory
target_compile_definitions(thesis-framework PRIVATE -DSHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shader/")

# external dependencies and libraries ------------------------------------

# include packages
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)

# add external libraries that are locally built: glad and imgui
add_subdirectory(ext)
target_include_directories(thesis-framework PRIVATE ext)

# link libraries
target_link_libraries(thesis-framework PRIVATE
    imgui
    glad
    glfw
    OpenGL::GL
#    GLUT::GLUT
)

