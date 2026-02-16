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
    GUI::GUI(ButtonController* controller) : buttons(controller), errorAvailable(false),
    m_showObjectInfo(false), m_selectedVertexId(0), m_objectType(0)
    {
        ErrorCollector::getInstance().signIn(std::ref(*this));
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

        if (errorAvailable)
        {
            errorPopup();
        }

        // vertex picking
        if (m_showObjectInfo) {
            objectIDPopup();
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
                           coords2.x, coords2.y, coords2.z);

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

    void GUI::objectIDPopup() {
        // Set position if we have valid screen coordinates
        if (m_popupPosition.x >= 0 && m_popupPosition.y >= 0) {
            ImGui::SetNextWindowPos(ImVec2(m_popupPosition.x, m_popupPosition.y),
                                    ImGuiCond_Appearing);
        } else {
            // Fallback to center of screen
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                           ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }

        if (ImGui::Begin("Object Information", &m_showObjectInfo)) {
            auto& graph = GraphSaver::getInstance().getGraph();
            if (m_objectType == 1) {
                ImGui::Text("Selected Vertex ID: %d", m_selectedVertexId);
                auto& vertex = graph.getVertexByID(m_selectedVertexId);
                glm::vec4 color = vertex.getVec4();

                ImGui::Text("Position: (%.2f, %.2f, %.2f)",
                            vertex.getCoordsVector()[0], vertex.getCoordsVector()[1], vertex.getCoordsVector()[2]);
                ImGui::Text("Color: (%.2f, %.2f, %.2f)",
                            color.r, color.g, color.b);
            } else if (m_objectType == 2) {
                ImGui::Text("Selected Edge ID: %d", m_selectedEdgeId);
                auto& edge = graph.getEdgeByID(m_selectedEdgeId);
                glm::vec4 color = edge.getVec4();
                auto [v1, v2] = edge.getConnectingVerticesIDs();
                ImGui::Text("Connects vertices: %u -%u", v1, v2);
                ImGui::Text("Color: (%.2f, %.2f, %.2f)", color.r, color.g, color.b);
            }
        }
        ImGui::End();
    }
}