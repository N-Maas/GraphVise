//
// Created by Emile Brückner on 1/19/26.
//

#include "GUI.hpp"

#include "controller/ErrorCollector.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace graphvise
{
    GUI::GUI(ButtonController* controller) : buttons(controller), errorAvailable(false)
    {
    }

    void GUI::initGUI(GLFWwindow* window)
    {
        // Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");


    }

    void GUI::loadFrame(int framebufferWidth, int framebufferHeight)
    {
        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        currentObjInfo();


        if (ImGui::Button("Press For Error"))
        {
            ErrorCollector::getInstance().collectError(Error(ErrorType::FILE_NOT_FOUND, "This is a Test Error Message"));
        }

        if (errorAvailable)
        {
            errorPopup();
        }

        ImGui::SetNextWindowPos(ImVec2(framebufferWidth, 19), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
        ImGui::Begin("##FPS window", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse
        );
        ImGui::Text("%.2f fps", fps);
        ImGui::End();




        buttons.loadButtonFrame(framebufferWidth, framebufferHeight);

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

    void GUI::setFps(double newFps)
    {
        fps = newFps;
    }

    void GUI::errorPopup()
    {
        ImGui::OpenPopup("Error", ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Message: %s", currentError->getErrorMessage().c_str());

        if (currentError->getMessage().has_value())
        {
            ImGui::Text("Info: %s",
                        currentError->getMessage()->c_str());
        }

        if (currentError->getLine().has_value())
        {
            ImGui::Separator();
            ImGui::Text("At line: %s",
                        currentError->getMessage()->c_str());
        }

        if (ImGui::Button("OK##Error Confirm"))
        {
            ImGui::CloseCurrentPopup();
            errorAvailable = false;
        }
        ImGui::EndPopup();
    }

    void GUI::currentObjInfo()
    {
        static Vertex currentVertex = GraphSaver::getInstance().getGraph().getVertexByID(currentObjId);


        ImGui::Begin("Current Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("ObjectID: %d", currentVertex.getID());
        const glm::vec3 coords = currentVertex.getCoordsVector();
        ImGui::Text("Coords: x: %.2f y: %.2f z: %.2f",
                    coords.x, coords.y, coords.z);
        const auto groupname = GraphSaver::getInstance().getGraph().getGroupByID(currentVertex.getConnectedGroupID()).getName();
        ImGui::Text("Object Group: %s", groupname.c_str());


        ImGui::Text("Object Color:");
        ImGui::SameLine();
        ImGui::ColorButton("##Vertex color", currentVertex.getVec4());
        ImGui::End();
    }

    void GUI::update()
    {
        errorAvailable = true;
        currentError = ErrorCollector::getInstance().getCurrentError();
    }
}
