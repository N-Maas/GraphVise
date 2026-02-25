//
// Created by Emile Brückner on 1/19/26.
//


#include "Buttons.hpp"
#include <format>
#include "imgui/imgui.h"
#include "imgui-filebrowser/imfilebrowser.h"

#define STB_IMAGE_IMPLEMENTATION

#include <functional>
#include <utility>

#include "stb_image.h"


namespace graphvise
{
    Buttons::Buttons(std::shared_ptr<ButtonController> buttonController) : buttonController(std::move(buttonController))
    {
    }

    void Buttons::initButtons()
    {
        importGroupConfigBrowser.SetTypeFilters(allowedGroupInfoFormat);
        highlightSubgraphBrowser.SetTypeFilters(allowedGroupInfoFormat);

        exportGraphBrowser.SetTypeFilters(allowedExportFormat);


        cameraBookmarkIcon = loadTextureFromFile(ICON_FILE_PATH "bookmark.png");
        randomize = loadTextureFromFile(ICON_FILE_PATH "Randomize Color button.png");
        searchIcon = loadTextureFromFile(ICON_FILE_PATH "Suche.png");
        groupIcon = loadTextureFromFile(ICON_FILE_PATH "Gruppen.png");
        performanceIcon = loadTextureFromFile(ICON_FILE_PATH "Performance.png");
        cameraMovementIcon = loadTextureFromFile(ICON_FILE_PATH "cameraMovement.png");
        lightSourceIcon = loadTextureFromFile(ICON_FILE_PATH "Light Source Switch Button.png");
    }

    void Buttons::loadButtonFrame(int framebufferWidth, int framebufferHeight)
    {
        this->framebufferWidth = framebufferWidth;
        this->framebufferHeight = framebufferHeight;

        ImGui::ShowDemoWindow();

        MainMenuBar();

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
        generalSettings();
        ImGui::Separator();
        help();
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

        const ImVec2 windowPivot = {1.0f, 0.5f};

        ImGui::SetNextWindowPos(pos, 0, windowPivot);


        ImGui::Begin("##SideBarMenu", nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoNavFocus
        );

        SideBarElement(searchIcon, "Search for Objects",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           findObject(hoverMsg);
                       }));

        SideBarElement(groupIcon, "Show Groups",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           GroupMenu(hoverMsg);
                       }));

        SideBarElement(performanceIcon, "Toggle Performance Mode",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           performanceModeToggle(hoverMsg);
                       }));

        SideBarElement(cameraBookmarkIcon, "Show Camera Bookmarks",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           cameraBookmarkMenu(hoverMsg);
                       }));


        if (ImGui::ImageButton(lightSourceIcon.id, ImVec2(50, 50)))
        {
            buttonController->toggleLightSourceMovementBehaviour();

        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Toggle Light Source");
        }
        ImGui::Spacing();



        if (ImGui::ImageButton(cameraMovementIcon.id, ImVec2(50, 50)))
        {
            buttonController->toggleCameraFocusMode();
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Toggle Camera Movement");
        }
        ImGui::Spacing();


        ImGui::End();
    }

    void Buttons::SideBarElement(const Texture texture, const char* hoverMsg,
                                 const std::function<void(const char* hoverMsg)>& onClickFunction)
    {
        if (ImGui::ImageButton(texture.id, ImVec2(50, 50)))
        {
            auto pos = ImGui::GetItemRectMin();
            pos.x -= 15;
            ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(1.0f, 0.0f));

            ImGui::OpenPopup(hoverMsg);
        }

        onClickFunction(hoverMsg);


        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(hoverMsg);
        }
        ImGui::Spacing();
    }

    void Buttons::changeObjSize()
    {
        static float cylRadPreCalc = sqrt(renderer->cylinderRadius);
        static float sphereRadPreCalc = sqrt(renderer->sphereRadius);

        if (ImGui::BeginMenu("Object Size"))
        {
            if (ImGui::DragFloat("Edge Size", &cylRadPreCalc, 0.001f, 0.001f, 10.0f))
            {
                renderer->cylinderRadius = pow(cylRadPreCalc, 2);
            }

            if (ImGui::DragFloat("Vertex Size", &sphereRadPreCalc, 0.001f, 0.001f, 10.0f))
            {
                renderer->sphereRadius = pow(sphereRadPreCalc, 2);
            }

            ImGui::EndMenu();
        }
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

    void Buttons::generalSettings()
    {
        if (ImGui::BeginMenu("General Settings"))
        {
            if (ImGui::BeginMenu("Change Background Color"))
            {
                ImGui::ColorEdit3("##Background Color", &renderer->backgroundColor.x);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }

    void Buttons::GroupMenu(const char* popUpName)
    {
        activeGroups = &saver->getGraph().getGroups();


        ImGui::SetNextWindowSizeConstraints({260, 300}, {MAXFLOAT, 300});
        if (ImGui::BeginPopup(popUpName,
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse
        ))
        {
            if (ImGui::Button("Randomize All Colors"))
            {
                buttonController->randomizeAllColors();
            }

            {
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
            }
            ImGui::EndPopup();
        }
    }

    void Buttons::ChangeTransparency(uint32_t groupID)
    {
        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        float& transparency = groupColors[groupID].first.w;

        if (transparency == 0.0f)
        {
            transparency = saver->getGraph().getGroupByID(groupID).getVec4().w;
        }

        if (ImGui::SliderFloat(std::format("##Transparency##{}", groupID).c_str(), &transparency, 0.0f, 1.0f))
        {
            if (transparency < 0.0f)
            {
                transparency = 0.0f;
            }
            else if (transparency > 1.0f)
            {
                transparency = 1.0f;
            }
            buttonController->changeTransparency(groupID, transparency);
        }
    }

    void Buttons::findObject(const char* popUpName)
    {
        if (ImGui::BeginPopup(popUpName,
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::Button("Remove Highlighting"))
            {
                buttonController->RemoveHighlights();
            }

            ImGui::BeginTabBar("##FindObjectTabBar");

            if (ImGui::BeginTabItem("Vertex"))
            {
                findVertex();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Edge"))
            {
                findEdge();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();

            ImGui::EndPopup();
        }
    }


    void Buttons::performanceModeToggle(const char* popUpName)
    {
        static const char* modeText[] = {"Quality", "Balance", "Performance"};

        if (ImGui::BeginPopup(popUpName,
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse
        ))
        {
            performanceMode = Renderer::getInstance()->performance_mode();


            if (ImGui::SliderInt("##ModeSlider", reinterpret_cast<int*>(&performanceMode),
                                 static_cast<int>(PerformanceMode::QUALITY),
                                 static_cast<int>(PerformanceMode::PERFORMANCE),
                                 modeText[static_cast<int>(performanceMode)]))
            {
                buttonController->setPerformanceMode(performanceMode);
            }


            ImGui::EndPopup();
        }
    }


    void Buttons::randomizeColoring(uint32_t groupID) const
    {
        if (ImGui::Button(std::format("Randomize Color ##{}", groupID).c_str()))
        {
            buttonController->randomizeColoring(groupID);
        }
    }

    void Buttons::changeColoring(uint32_t groupID)
    {
        if (groupID >= groupColors.size())
        {
            groupColors.resize(groupColors.size() * 2);
        }

        ImVec4& new_color = groupColors[groupID].first;
        ImVec4& old_color = groupColors[groupID].second;

        if (new_color.x == 0 && new_color.y == 0 && new_color.z == 0 && new_color.w == 0)
        {
            new_color = saver->getGraph().getGroupByID(groupID).getVec4();
            old_color = new_color;
        }


        if (ImGui::ColorEdit3(std::format("##Change Color Edit{}", groupID).c_str(), &new_color.x))
        {
            buttonController->changeColoring(groupID, new_color);
        }
        if (ImGui::Button(std::format("Revert##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, old_color);
            new_color = old_color;
        }
        ImGui::SameLine();
        if (ImGui::Button(std::format("Apply##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, new_color);
            old_color = new_color;
        }
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

    void Buttons::highlightSubgraph()
    {
        if (ImGui::MenuItem("Highlight Subgraph"))
        {
            highlightSubgraphBrowser.SetTitle("Highlight Subgraph");
            highlightSubgraphBrowser.Open();
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
            ImGui::EndMenu();
        }

        if (importGraphBrowser.HasSelected())
        {
            const std::filesystem::path result = importGraphBrowser.GetSelected();


            buttonController->importGraph(result, importFormat);

            importGraphBrowser.ClearSelected();
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
            const auto filename = exportGraphBrowser.GetSelected();


            buttonController->exportGraph(filename, exportFormat);
            exportGraphBrowser.ClearSelected();
        }
    }

    void Buttons::cameraBookmarkMenu(const char* popUpName)
    {
        auto bookmarks = saver->getGraph().getCameraBookmarks();

        auto x = ImGui::CalcTextSize("Position: -231.22, -231.22, -231.22").x;
        ImGui::SetNextWindowSizeConstraints(ImVec2(x, 0), ImVec2(x, MAXFLOAT));

        if (ImGui::BeginPopup(popUpName,
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse
        ))
        {
            if (ImGui::Button("Add Bookmark"))
            {
                addBookmarkWindow = true;
            }
            for (size_t bookmarkID = 0; bookmarkID < bookmarks.size(); ++bookmarkID)
            {
                ImGui::Separator();

                auto& bookmark = bookmarks[bookmarkID];

                ImGui::Text(bookmark.getName().c_str());
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
            ImGui::EndPopup();
        }

        if (addBookmarkWindow)
        {
            ImGui::OpenPopup("Add Bookmark");
        }

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

    void Buttons::importGroupConfiguration()
    {
        if (ImGui::MenuItem("Import Group Config"))
        {
            importGroupConfigBrowser.SetTitle("Import Group Config");
            importGroupConfigBrowser.Open();
        }
    }

    void Buttons::help()
    {
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::BeginMenu("Info"))
            {
                const auto infoText = "This is a simple 3D graph viewer. \n"
                    "The viewer supports the following operations: \n"
                    " - Import graphs from .txt and .cnf files \n"
                    " - Highlight subgraphs \n"
                    " - Change the coloring of groups \n"
                    " - Change the transparency of groups \n"
                    " - Find vertices and edges in the graph \n"
                    " - Export the graph as a PNG image \n";

                ImGui::Text(infoText);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Hotkeys"))
            {
                const auto hotkeyText = "- L-CTRL + K : Change Camera Movement Mode \n"
                    "- L-CTRL + Q : Rotate through Rendering Qualities \n"
                    "- L-CTRL + L : Toggle Light Source Behavior \n";

                ImGui::Text(hotkeyText);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
    }

    Texture Buttons::loadTextureFromFile(const char* filename)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);


        if (!data)
        {
            return Texture(nullptr, 0, 0);
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
        return Texture(reinterpret_cast<ImTextureID>(texture), width, height);
    }
}
