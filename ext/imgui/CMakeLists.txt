cmake_minimum_required(VERSION 3.16)
project(Imgui)

# library target that we can link / include in the parent directory
add_library(imgui
    imconfig.h
    imgui.h
    imgui_impl_glfw.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3_loader.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
#
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)


target_include_directories(imgui PUBLIC include/)
