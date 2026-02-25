//
// Created by Emile Brückner on 1/19/26.
//

#include "GUI.hpp"

#include <iostream>

#include "controller/ErrorCollector.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace graphvise
{
    GUI::GUI(const std::shared_ptr<ButtonController>& controller) : buttons(controller), errorAvailable(false)
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

        buttons.initButtons();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    }

    void GUI::loadFrame(int framebufferWidth, int framebufferHeight)
    {
        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentObjInfo();   //vertex and edge picking

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
                        std::to_string(currentError->getLine().value()).c_str());
        }

        if (ImGui::Button("OK##Error Confirm"))
        {
            ImGui::CloseCurrentPopup();
            errorAvailable = false;
        }
        ImGui::EndPopup();
    }

    void GUI::currentObjInfo() {

        ImGui::Begin("Current Objects", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginTabBar("##CurrentObjTabBar");

        auto& graph = GraphSaver::getInstance().getGraph();
        if (ImGui::BeginTabItem("Current Vertex")) {

            if (m_objectType == 1) {
                const auto& currentVertex = graph.getVertexByID(m_selectedVertexId);
                ImGui::Text("Vertex ID: %d", m_selectedVertexId);
                const glm::vec3 coords = currentVertex.getCoordsVector();
                ImGui::Text("Coords: x: %.2f y: %.2f z: %.2f",
                            coords.x, coords.y, coords.z);
                const auto group = GraphSaver::getInstance().getGraph().getGroupByID(currentVertex.getConnectedGroupID());
                const auto groupname = group.getName();
                ImGui::Text("Vertex Group: %s", groupname.c_str());


                ImGui::Text("Vertex Color:");
                ImGui::SameLine();
                ImGui::ColorButton("##Vertex color", group.getVec4());
            } else {
                ImGui::Text("No vertex selected.");
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Current Edge")) {
            if (m_objectType == 2) {
                const auto& currentEdge = graph.getEdgeByID(m_selectedEdgeId);
                const auto& vertexIDs = currentEdge.getConnectingVerticesIDs();
                const auto& firstVertex = graph.getVertexByID(vertexIDs.first);
                const auto& secondVertex = graph.getVertexByID(vertexIDs.second);

                ImGui::Text("Edge ID: %d", m_selectedEdgeId);
                const glm::vec3 coords = firstVertex.getCoordsVector();
                ImGui::Text("Vertex 1 Coords: x: %.2f y: %.2f z: %.2f",
                            coords.x, coords.y, coords.z);
                const glm::vec3 coords2 = secondVertex.getCoordsVector();
                ImGui::Text("Vertex 2 Coords: x: %.2f y: %.2f z: %.2f",
                           coords2.x, coords2.y, coords2.z);

                const auto group = GraphSaver::getInstance().getGraph().getGroupByID(currentEdge.getConnectedGroupID());
                const auto groupname = group.getName();
                ImGui::Text("Edge Group: %s", groupname.c_str());


                ImGui::Text("Edge Color:");
                ImGui::SameLine();
                ImGui::ColorButton("##Edge color", group.getVec4());
            } else {
                ImGui::Text("No edge selected.");
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
        ImGui::End();
    }

    void GUI::update()
    {
        std::cout << "updated" << std::endl;
        errorAvailable = true;
        currentError = ErrorCollector::getInstance().getCurrentError();
    }

    void GUI::showVertexInfo(uint32_t vertexId) {
        m_selectedVertexId = vertexId;
        m_selectedEdgeId = UINT32_MAX;
        m_showObjectInfo = true;
        m_objectType = 1; // object type vertex
    }

    void GUI::showEdgeInfo(uint32_t edgeId) {
        m_selectedVertexId = UINT32_MAX;
        m_selectedEdgeId = edgeId;
        m_showObjectInfo = true;
        m_objectType = 2; // object type edge
    }


}