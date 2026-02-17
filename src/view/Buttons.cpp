//
// Created by Emile Brückner on 1/19/26.
//


#include "Buttons.hpp"
#include <format>
#include "imgui/imgui.h"
#include "imgui-filebrowser/imfilebrowser.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "controller/Exporting/CachingController.hpp"

#define MainMenuBarHeight 19
#define findObjectHeight 115
#define GroupMenuHeight 300
#define PerformanceHeight 54
#define MovementLightSourceHeight 77
#define MovementCameraHeight 77


namespace graphvise
{
    Buttons::Buttons(ButtonController* controller)
    {
        this->buttonController = controller;

        importGroupConfigBrowser.SetTypeFilters(allowedGroupInfoFormat);
        highlightSubgraphBrowser.SetTypeFilters(allowedGroupInfoFormat);

        exportGraphBrowser.SetTypeFilters(allowedExportFormat);
    }

    void Buttons::loadButtonFrame(int framebufferWidth, int framebufferHeight)
    {
        this->framebufferWidth = framebufferWidth;
        this->framebufferHeight = framebufferHeight;

        // ImGui::ShowDemoWindow();

        MainMenuBar();

        CachingController::loadLastCachedGraph();

        SideBar();
    }

    void Buttons::MainMenuBar()
    {
        // Main Menu Bar at the top of the Window
        ImGui::BeginMainMenuBar();


        importGraph();
        ImGui::Separator();
        exportGraph();
        ImGui::Separator();
        graphSettings();
        ImGui::Separator();


        ImGui::EndMainMenuBar();

        importGraphBrowser.Display();
        importGroupConfigBrowser.Display();
        highlightSubgraphBrowser.Display();
        exportGraphBrowser.Display();
    }

    void Buttons::SideBar()
    {
        ImVec2 pos;
        pos.x = static_cast<float>(framebufferWidth);
        pos.y = static_cast<float>(framebufferHeight) / 2.0f;

        ImVec2 windowPivot = {1.0f, 0.5f};

        ImGui::SetNextWindowPos(pos, 0, windowPivot);


        ImGui::Begin("##SideBarMenu", nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoTitleBar
        );

        SideBarElement(searchIcon, "Search for Objects", &search);
        SideBarElement(groupIcon, "Show Groups", &groups);
        SideBarElement(performanceIcon, "Toggle Performance Mode", &togglePerformanceMode);
        SideBarElement(lightSourceIcon, "Toggle Light Source", &lightSource);
        SideBarElement(cameraMovementIcon, "Toggle Camera Movement", &cameraMovement);
        SideBarElement(cameraBookmarkIcon, "Show Camera Bookmarks", &cameraBookmarks);


        const ImVec2 sideBarSize = ImGui::GetWindowSize();
        ImGui::End();

        constexpr float widgetSpacing = 3.0f;

        pos.x = pos.x - sideBarSize.x - 20;
        pos.y = MainMenuBarHeight;

        windowPivot = {1.0f, 0.0f};

        if (search)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);
            findObject();
        }

        pos.y += findObjectHeight + widgetSpacing;

        if (groups)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);
            GroupMenu(&groups);
        }

        pos.y += GroupMenuHeight + widgetSpacing;

        if (togglePerformanceMode)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            performanceModeToggle(&togglePerformanceMode);
        }
        pos.y += PerformanceHeight + widgetSpacing;

        if (lightSource)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            setLightSourceMovementBehaviour(&lightSource);
        }

        pos.y += MovementLightSourceHeight + widgetSpacing;
        if (cameraMovement)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);

            setCameraMovementMode(&cameraMovement);
        }

        pos.y += MovementCameraHeight + widgetSpacing;

        if (cameraBookmarks)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, windowPivot);
            cameraBookmarkMenu(&cameraBookmarks);
        }
    }

    void Buttons::SideBarElement(const Texture texture, const char* hoverMsg, bool* state)
    {
        if (ImGui::ImageButton(texture.id, ImVec2(50, 50)))
        {
            *state = !*state;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(hoverMsg);
        }
        ImGui::Spacing();
    }

    void Buttons::graphSettings()
    {
        if (ImGui::BeginMenu("Graph Settings"))
        {
            highlightSubgraph();
            ImGui::Separator();
            importGroupConfiguration();
            ImGui::Separator();
            changeObjSize();


            ImGui::EndMenu();
        }

        if (highlightSubgraphBrowser.HasSelected())
        {
            const std::filesystem::path result = highlightSubgraphBrowser.GetSelected();

            buttonController->highlightSubgraph(result);

            highlightSubgraphBrowser.ClearSelected();
        }

        if (importGroupConfigBrowser.HasSelected())
        {
            const std::filesystem::path result = importGroupConfigBrowser.GetSelected();

            buttonController->importGroupConfiguration(result);

            importGroupConfigBrowser.ClearSelected();
        }
    }

    Texture Buttons::loadTextureFromFile(const char* filename)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);


        if (!data)
        {
            return Texture(0, 0, 0);
        }

        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
        return Texture((ImTextureID)texture, width, height);
    }

    void Buttons::changeObjSize()
    {
        if (ImGui::BeginMenu("Object Size"))
        {
            if (ImGui::DragFloat("Edge Size", &cylinderRadius, 0.001f, 0.001f, 1.0f))
            {
                Renderer::getInstance()->setCylinderRadius(cylinderRadius);
            };
            if (ImGui::DragFloat("Vertex Size", &sphereRadius, 0.001f, 0.001f, 1.0f))
            {
                Renderer::getInstance()->setSphereRadius(sphereRadius);
            }

            ImGui::EndMenu();
        }
    }

    void Buttons::cameraBookmarkMenu(bool* visible)
    {
        auto bookmarks = saver->getGraph().getCameraBookmarks();

        auto x = ImGui::CalcTextSize("Position: -231.22, -231.22, -231.22").x;
        ImGui::SetNextWindowSizeConstraints(ImVec2(x, 0), ImVec2(x, MAXFLOAT));

        ImGui::Begin("Bookmarks", visible,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::Button("Add Bookmark"))
        {
            addBookmarkWindow = true;
        }
        for (size_t bookmarkID = 0; bookmarkID < bookmarks.size(); ++bookmarkID)
        {
            auto& bookmark = bookmarks[bookmarkID];
            if (ImGui::CollapsingHeader(std::format("{}##{}", bookmark.getName(), bookmarkID).c_str()))
            {
                ImGui::Text("Position: %.2f, %.2f, %.2f", bookmark.getCoordsVector().x, bookmark.getCoordsVector().y,
                            bookmark.getCoordsVector().z);
                if (ImGui::Button(std::format("Load Bookmark##{}", bookmarkID).c_str()))
                {
                    buttonController->loadCameraBookmark(bookmark);
                }
                ImGui::SameLine();
                if (ImGui::Button(std::format("Delete Bookmark##{}", bookmarkID).c_str()))
                {
                    buttonController->deleteCameraBookmark(bookmarkID);
                }
            }
        }
        ImGui::End();

        if (addBookmarkWindow)
        {
            ImGui::OpenPopup("Add Bookmark",
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoCollapse
            );
            if (ImGui::BeginPopupModal("Add Bookmark", &addBookmarkWindow,
                                       ImGuiWindowFlags_AlwaysAutoResize |
                                       ImGuiWindowFlags_NoCollapse))
            {
                ImGui::InputText("Name", bookmarkName.data(), bookmarkName.size());
                if (ImGui::Button("Add"))
                {
                    buttonController->addCurrentPosAsBookmark(std::string(bookmarkName.data()));
                    bookmarkName = std::vector<char>(16);
                    addBookmarkWindow = false;
                }
                ImGui::EndPopup();
            }
        }
    }

    void Buttons::GroupMenu(bool* groupMenu)
    {
        activeGroups = &saver->getGraph().getGroups();

        ImGui::SetNextWindowSizeConstraints({260, 300}, {MAXFLOAT, 300});
        ImGui::Begin("Groups", groupMenu,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );


        for (const auto& group : *activeGroups)
        {
            if (ImGui::CollapsingHeader(group.getName().c_str()))
            {
                ImGui::Text("Group ID: %d", group.getID());
                ImGui::SameLine();
                ImGui::ColorButton(std::format("Group Color##{}", group.getID()).c_str(), group.getVec4());
                ImGui::SameLine();

                randomizeColoring(group.getID());
                changeColoring(group.getID());

                ChangeTransparency(group.getID());
            }
        }

        ImGui::End();
    }


    void Buttons::ChangeTransparency(uint32_t groupID)
    {
        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        float& transparency = groupColors[groupID].w;

        if (transparency == 0.0f)
        {
            transparency = saver->getGraph().getGroupByID(groupID).getVec4().w;
        }

        if (ImGui::SliderFloat(std::format("##Transparency##{}", groupID).c_str(), &transparency, 0.0f, 1.0f))
        {
            buttonController->changeTransparency(groupID, transparency);
        }
    }


    void Buttons::changeColoring(uint32_t groupID)
    {
        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        ImVec4& color = groupColors[groupID];

        if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0)
        {
            color = saver->getGraph().getGroupByID(groupID).getVec4();
        }


        ImGui::ColorEdit3(std::format("##Change Color Edit{}", groupID).c_str(), &color.x);
        if (ImGui::Button(std::format("Change Color##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, color);
        }
    }

    void Buttons::randomizeColoring(uint32_t groupID) const
    {
        if (ImGui::Button(std::format("Randomize Color ##{}", groupID).c_str()))
        {
            buttonController->randomizeColoring(groupID);
        }
    }

    void Buttons::performanceModeToggle(bool* toggle_mode)
    {
        const char* modeText[] = {"High Performance", "High Resolution"};

        ImGui::Begin("Performance Mode", toggle_mode,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::SliderInt("##ModeSlider", reinterpret_cast<int*>(&performanceMode),
                             HIGH_PERFORMANCE, HIGH_RESOLUTION, modeText[performanceMode]))
        {
            buttonController->togglePerformanceMode(performanceMode);
        }


        ImGui::End();
    }


    void Buttons::setLightSourceMovementBehaviour(bool* lightSourceMovementBehaviorToggle)
    {
        ImGui::Begin("Light Source", lightSourceMovementBehaviorToggle,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );
        const bool first = ImGui::RadioButton("Fixed Position", reinterpret_cast<int*>(&lightSourceMovementBehaviour),
                                              FIXED_POSITION);
        const bool second = ImGui::RadioButton("Follow Camera", reinterpret_cast<int*>(&lightSourceMovementBehaviour),
                                               FOLLOW_CAMERA);

        if (first || second)
        {
            buttonController->setLightSourceMovementBehaviour(lightSourceMovementBehaviour);
        }


        ImGui::End();
    }

    void Buttons::setCameraMovementMode(bool* cameraMovementMode)
    {
        ImGui::Begin("Camera Movement", cameraMovementMode,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        const bool first = ImGui::RadioButton("Free Camera", reinterpret_cast<int*>(&cameraMode), FREE);
        const bool second = ImGui::RadioButton("Center of Mass", reinterpret_cast<int*>(&cameraMode), CENTER_OF_MASS);

        if (first || second)
        {
            buttonController->setCameraFocusMode(cameraMode);
        }


        ImGui::End();
    }

    void Buttons::findObject()
    {
        ImGui::Begin("Highlight Object", &search,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::Button("Remove Highlighting"))
        {
            buttonController->RemoveHighlights();
        }

        ImGui::BeginTabBar("##FindObjectTabBar");

        if (ImGui::BeginTabItem("Edge"))
        {
            findEdge();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Vertex"))
        {
            findVertex();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();

        ImGui::End();
    }

    void Buttons::findVertex()
    {
        ImGui::InputInt("##VertexID", &vertex);
        if (ImGui::Button("Find Vertex"))
        {
            buttonController->findVertex(vertex);
            vertex = 0;
        }
    }

    void Buttons::findEdge()
    {
        ImGui::InputInt2("##Vertex IDs:", edgeVertices);
        if (ImGui::Button("Find Edge"))
        {
            buttonController->findEdge(edgeVertices[0], edgeVertices[1]);
            edgeVertices[0] = 0;
            edgeVertices[1] = 0;
        }
    }

    void Buttons::importGraph()
    {
        if (ImGui::BeginMenu("import Graph"))
        {
            if (ImGui::MenuItem("Import as TXT"))
            {
                importFormat = ImportFormat::TXT;
                importGraphBrowser.SetTypeFilters(txtImportFormat);
                importGraphBrowser.SetTitle("import Graph from .txt");
                importGraphBrowser.Open();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Import as CNF"))
            {
                importFormat = ImportFormat::CNF;
                importGraphBrowser.SetTypeFilters(cnfImportFormat);

                importGraphBrowser.SetTitle("import Graph from .cnf");
                importGraphBrowser.Open();
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Load Graph from Cache"))
            {

                auto filenames = CachingController::getCachedGraphFilenames();

                for (auto const& file : filenames)
                {
                    if (ImGui::MenuItem(file.stem().c_str()))
                    {
                        CachingController::loadCachedGraph(file);
                    }
                }

                ImGui::EndMenu();
            }


            if (CachingController::hasCachedGraphs())
            {
                ImGui::Separator();

                if (ImGui::MenuItem("Load last loaded Graph"))
                {
                    CachingController::loadLastCachedGraph();
                }
            }

            ImGui::EndMenu();
        }

        if (importGraphBrowser.HasSelected())
        {
            const std::filesystem::path result = importGraphBrowser.GetSelected();


            buttonController->importGraph(result, importFormat);

            importGraphBrowser.ClearSelected();
        }
    }

    void Buttons::highlightSubgraph()
    {
        if (ImGui::MenuItem("Highlight Subgraph"))
        {
            highlightSubgraphBrowser.SetTitle("Highlight Subgraph");
            highlightSubgraphBrowser.Open();
        }
    }

    void Buttons::importGroupConfiguration()
    {
        if (ImGui::MenuItem("Import Group Config"))
        {
            importGroupConfigBrowser.SetTitle("Import Group Config");
            importGroupConfigBrowser.Open();
        }
    }

    void Buttons::exportGraph()
    {
        exportGraphBrowser.SetTitle("Choose Export Location");

        if (ImGui::BeginMenu("Export Graph"))
        {
            if (ImGui::MenuItem("Export as PNG"))
            {
                exportFormat = ExportFormat::PNG;
                exportGraphBrowser.SetInputName("graph.png");
                exportGraphBrowser.Open();
            }
            // if (ImGui::MenuItem("Export as JPG"))
            // {
            //
            //     exportGraphBrowser.SetInputName("graph.jpg");
            //     exportGraphBrowser.Open();
            //
            // }

            ImGui::EndMenu();
        }


        if (exportGraphBrowser.HasSelected())
        {
            auto filename = exportGraphBrowser.GetSelected();


            buttonController->exportGraph(filename, exportFormat);
            exportGraphBrowser.ClearSelected();
        }
    }
}
