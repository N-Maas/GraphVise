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

        if (ThreadController::backGroundThreadBusy())
        {
            ImGui::SetNextWindowPos(ImVec2(0, framebufferHeight), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
            ImGui::Begin("##Loading window", nullptr,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse
            );
            ImGui::Text("Loading...");
            ImGui::End();
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
        auto graph = GraphSaver::getInstance().getGraph();

        auto currentVertexID = graph.getCurrentVertexID();
        auto currentEdgeID = graph.getCurrentEdgeID();

        ImGui::Begin("Current Objects", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginTabBar("##CurrentObjTabBar");

        if (ImGui::BeginTabItem("Current Vertex"))
        {
                if (!currentVertexID.has_value())
                {
                    ImGui::Text("No vertex selected.");
                } else
                {
                    const auto& currentVertex = graph.getVertexByID(currentVertexID.value());
                    ImGui::Text("Vertex ID: %d", currentVertex.getID());
                    const glm::vec3 coords = currentVertex.getCoordsVector();
                    ImGui::Text("Coords: x: %.2f y: %.2f z: %.2f",
                                coords.x, coords.y, coords.z);
                    const auto groupname = GraphSaver::getInstance().getGraph().getGroupByID(currentVertex.getConnectedGroupID()).getName();
                    ImGui::Text("Vertex Group: %s", groupname.c_str());


                    ImGui::Text("Vertex Color:");
                    ImGui::SameLine();
                    ImGui::ColorButton("##Vertex color", currentVertex.getVec4());
                }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Current Edge"))
        {
            if (!currentEdgeID.has_value())
            {
                ImGui::Text("No edge selected.");
            } else

            {
                const auto& currentEdge = graph.getEdgeByID(currentEdgeID.value());
                const auto& vertexIDs = currentEdge.getConnectingVerticesIDs();
                const auto& firstVertex = graph.getVertexByID(vertexIDs.first);
                const auto& secondVertex = graph.getVertexByID(vertexIDs.second);

                ImGui::Text("Edge ID: %d", currentEdge.getID());
                const glm::vec3 coords = firstVertex.getCoordsVector();
                ImGui::Text("Vertex 1 Coords: x: %.2f y: %.2f z: %.2f",
                            coords.x, coords.y, coords.z);
                    const glm::vec3 coords2 = secondVertex.getCoordsVector();
                ImGui::Text("Vertex 2 Coords: x: %.2f y: %.2f z: %.2f",
                           coords.x, coords.y, coords.z);

                const auto groupname = GraphSaver::getInstance().getGraph().getGroupByID(currentEdge.getConnectedGroupID()).getName();
                ImGui::Text("Edge Group: %s", groupname.c_str());


                ImGui::Text("Edge Color:");
                ImGui::SameLine();
                ImGui::ColorButton("##Edge color", currentEdge.getVec4());
            }
            ImGui::EndTabItem();

        }
        ImGui::EndTabBar();
        ImGui::End();
        }

    void GUI::update()
    {
        errorAvailable = true;
        currentError = ErrorCollector::getInstance().getCurrentError();
    }
}
