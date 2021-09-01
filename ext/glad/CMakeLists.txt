cmake_minimum_required(VERSION 3.16)
project(Glad VERSION 3.3 DESCRIPTION "Glad 3.3 Core OpenGL profile")

# library target that we can link / include in the parent directory
add_library(glad include/glad/glad.h src/glad.c)

target_link_libraries(glad ${CMAKE_DL_LIBS})

target_include_directories(glad PUBLIC include/)
