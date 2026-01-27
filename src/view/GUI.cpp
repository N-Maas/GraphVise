//
// Created by Emile Brückner on 1/19/26.
//

#include "GUI.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace graphvise {
    GUI::GUI()
    {

    }

    void GUI::initGUI(GLFWwindow* window)
    {
        // Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");



    }
    void GUI::loadFrame()
    {
        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        buttons.loadButtonFrame();
        currentObjInfo();

        errorPopup();

        // Renders the ImGUI elements
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



    }
    void GUI::shutdownGUI()
    {

        // Deletes all ImGUI instances
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void GUI::loadGUI()
    {
    }

    void GUI::errorPopup()
    {

        static bool errorAvailable = false;

        if (errorAvailable)
        {
            ImGui::OpenPopup("Error", ImGuiWindowFlags_AlwaysAutoResize );
            ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Wrong Text Format");
            ImGui::Separator();

            if (ImGui::Button("Ok"))
            {
                ImGui::CloseCurrentPopup();
                errorAvailable = false;
            }
            ImGui::EndPopup();
        }

        ImGui::Begin("Error Showing");
        ImGui::Text("Want to show Error?");
        if (ImGui::Button("Yes"))
        {
            errorAvailable = true;
        }
        ImGui::End();

    }

    void GUI::currentObjInfo() {
        static constexpr int id = 15;

        static auto vertexName = "ExampleVertex";
        static auto groupName = "Main Group";
        static auto coords = glm::vec3(100.2f, 122.0f, -123.0f);
        static const ImVec4 exampleColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);



        ImGui::Begin("Current Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("ObjectID: %d \n ", id);
        ImGui::Text("Coords: x: %.2f y: %.2f z: %.2f",
            coords.x, coords.y, coords.z);
        ImGui::Text("Object Name: %s \n ", vertexName);
        ImGui::Text("Object Group: %s \n ", groupName);
        ImGui::Text("Object Color:"); ImGui::SameLine(); ImGui::ColorButton("", exampleColor);
        ImGui::End();

    }
}