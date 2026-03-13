//
// Created by Emile Brückner on 1/19/26.
//

#include "GUI.hpp"

#include "controller/ErrorCollector.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "model/GraphSaver.hpp"

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
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        buttons.initButtons();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    }

    void GUI::loadFrame(const int framebufferWidth, const int framebufferHeight)
    {
        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentObjInfo(framebufferWidth, framebufferHeight); //vertex and edge picking

        if (ThreadController::backGroundThreadBusy())
        {
            ImGui::SetNextWindowPos(ImVec2(0, framebufferHeight), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
            ImGui::Begin("##Loading window", nullptr,
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoTitleBar |
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

        ImGui::Text( currentError->getErrorMessage().c_str());

        if (ImGui::Button("OK##Error Confirm"))
        {
            ImGui::CloseCurrentPopup();
            errorAvailable = false;
        }
        ImGui::EndPopup();
    }

    void GUI::currentObjInfo(const int framebufferWidth, const int framebufferHeight)
    {
        constexpr const char* popupName = "Current Object Info";

        auto graph = GraphSaver::getInstance().getGraph();
            switch (m_objectType)
            {
                case ObjType::VERTEX:
                {
                    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(10, 65, 110, 255));
                    ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(10, 65, 110, 255));
                    ImGui::SetNextWindowPos({(static_cast<float>(framebufferWidth) / 2 - 210),(static_cast<float>(framebufferHeight) - 125)});
                    ImGui::SetNextWindowSize({420, 120});
                    if (ImGui::Begin(popupName, nullptr,
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoInputs |
                        ImGuiWindowFlags_NoNavFocus))
                        {
                        const auto& currentVertex = graph.getVertexByID(m_selectedVertexId);
                        ImGui::Text("Current Vertex ID: %d", m_selectedVertexId);
                        const glm::vec3 coords = currentVertex.getCoordsVector();
                        ImGui::Text("Coords: x: %.2f y: %.2f z: %.2f",
                                    coords.x, coords.y, coords.z);
                        const auto group = GraphSaver::getInstance().getGraph().getGroupByID(
                            currentVertex.getConnectedGroupID());
                        const auto& groupname = group.getName();
                        ImGui::Text("Vertex Group: %s", groupname.c_str());


                        ImGui::Text("Vertex Color:");
                        ImGui::SameLine();
                        ImGui::ColorButton("##Vertex color", group.getVec4());
                    }
                    ImGui::End();
                    break;
                }

                case ObjType::EDGE:
                {
                    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(10, 65, 110, 255));
                    ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(10, 65, 110, 255));
                    ImGui::SetNextWindowPos({(static_cast<float>(framebufferWidth) / 2 - 210),(static_cast<float>(framebufferHeight) - 125)});
                    ImGui::SetNextWindowSize({420, 120});
                    if (ImGui::Begin(popupName, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNavFocus)) {
                        const auto& currentEdge = graph.getEdgeByID(m_selectedEdgeId);
                        const auto& [fst, snd] = currentEdge.getConnectingVerticesIDs();
                        const auto& firstVertex = graph.getVertexByID(fst);
                        const auto& secondVertex = graph.getVertexByID(snd);

                        ImGui::Text("Current Edge ID: %d", m_selectedEdgeId);
                        const glm::vec3 coords = firstVertex.getCoordsVector();
                        ImGui::Text("1. Vertex (ID: %d), Coords: x: %.2f y: %.2f z: %.2f", firstVertex.getID(),
                                    coords.x, coords.y, coords.z);
                        const glm::vec3 coords2 = secondVertex.getCoordsVector();
                        ImGui::Text("2. Vertex (ID: %d), Coords: x: %.2f y: %.2f z: %.2f", secondVertex.getID(),
                                    coords2.x, coords2.y, coords2.z);

                        const auto group = GraphSaver::getInstance().getGraph().getGroupByID(
                            currentEdge.getConnectedGroupID());
                        const auto& groupname = group.getName();
                        ImGui::Text("Edge Group: %s", groupname.c_str());


                        ImGui::Text("Edge Color:");
                        ImGui::SameLine();
                        ImGui::ColorButton("##Edge color", group.getVec4());
                    }
                    ImGui::End();
                    break;
                }
                default: {
                    m_objectType = ObjType::NONE;
                    break;
                }
            }
    }

    void GUI::update()
    {
        errorAvailable = true;
        currentError = ErrorCollector::getInstance().getCurrentError();
    }

    void GUI::showVertexInfo(const uint32_t vertexId)
    {
        m_selectedVertexId = vertexId;
        m_selectedEdgeId = UINT32_MAX;
        m_objectType = ObjType::VERTEX;
    }

    void GUI::showEdgeInfo(const uint32_t edgeId)
    {
        m_selectedVertexId = UINT32_MAX;
        m_selectedEdgeId = edgeId;
        m_objectType = ObjType::EDGE;
    }

    void GUI::resetObjInfo() {
        m_objectType = ObjType::NONE;
    }
}
