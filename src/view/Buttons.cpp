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
#include "controller/Exporting/CachingController.hpp"
#include "imgui/imgui_internal.h"


namespace graphvise
{
    Buttons::Buttons(std::shared_ptr<ButtonController> buttonController) : buttonController(std::move(buttonController))
    {
    }

    void Buttons::initButtons()
    {

        // Initialize file browsers with allowed formats
        importGroupConfigBrowser.SetTypeFilters(allowedGroupInfoFormat);
        highlightSubgraphBrowser.SetTypeFilters(allowedGroupInfoFormat);
        exportGraphBrowser.SetTypeFilters(allowedExportFormat);


        // Load icons for buttons
        cameraBookmarkIcon = loadTextureFromFile(ICON_FILE_PATH "bookmark.png");
        randomize = loadTextureFromFile(ICON_FILE_PATH "Randomize Color button.png");
        searchIcon = loadTextureFromFile(ICON_FILE_PATH "Suche.png");
        groupIcon = loadTextureFromFile(ICON_FILE_PATH "Gruppen.png");


        performanceIcon_Balanced = loadTextureFromFile(ICON_FILE_PATH "Performance_Balance.png");
        performanceIcon_Quality = loadTextureFromFile(ICON_FILE_PATH "Performance_Quality.png");
        performanceIcon_Performance = loadTextureFromFile(ICON_FILE_PATH "Performance_Performance.png");
        currentPerformanceMode = performanceIcon_Quality;

        cameraMovementIcon_CenterOfMass = loadTextureFromFile(ICON_FILE_PATH "CameraMode1.png");
        cameraMovementIcon_Free = loadTextureFromFile(ICON_FILE_PATH "CameraMode2.png");
        currentCameraIcon = cameraMovementIcon_CenterOfMass;

        lightSourceIcon_Fixed = loadTextureFromFile(ICON_FILE_PATH "Light Source Switch Button1.png");
        lightSourceIcon_FollowCamera = loadTextureFromFile(ICON_FILE_PATH "Light Source Switch Button2.png");
        currentLightSourceIcon = lightSourceIcon_Fixed;
    }

    void Buttons::loadButtonFrame(int framebufferWidth, int framebufferHeight)
    {
        this->framebufferWidth = framebufferWidth;
        this->framebufferHeight = framebufferHeight;


        MainMenuBar();

        SideBar();

        switch (*performanceMode)
        {
        case PerformanceMode::QUALITY:
            currentPerformanceMode = performanceIcon_Quality;
            break;
        case PerformanceMode::BALANCE:
            currentPerformanceMode = performanceIcon_Balanced;
            break;
        case PerformanceMode::PERFORMANCE:
            currentPerformanceMode = performanceIcon_Performance;
            break;
            default:
            throw std::runtime_error("Invalid Performance Mode");
        }

        switch (*cameraMode)
        {
        case CameraFocusMode::CENTER_OF_MASS:
            currentCameraIcon = cameraMovementIcon_CenterOfMass;
            break;
        case CameraFocusMode::FREE:
            currentCameraIcon = cameraMovementIcon_Free;
			break;
            default:
            break;
        }
        switch (*lightSourceMovementBehaviour)
        {
        case LightSourceMovementBehaviour::FIXED_POSITION:
            currentLightSourceIcon = lightSourceIcon_Fixed;
            break;
        case LightSourceMovementBehaviour::FOLLOW_CAMERA:
            currentLightSourceIcon = lightSourceIcon_FollowCamera;
			break;
            default:
            throw std::runtime_error("Invalid Light Source Movement Mode");
        }
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

        ImGui::SameLine(1180);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "Current Graph: %s", saver->getGraph().getName().c_str());

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
                     ImGuiWindowFlags_NoNav
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

        SideBarElement(currentPerformanceMode, "Toggle Performance Mode (Q)",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           performanceModeToggle(hoverMsg);
                       }));

        SideBarElement(cameraBookmarkIcon, "Show Camera Bookmarks",
                       std::function<void(const char* hoverMsg)>([this](const char* hoverMsg)
                       {
                           cameraBookmarkMenu(hoverMsg);
                       }));


        if (ImGui::ImageButton(currentLightSourceIcon.id, ImVec2(50, 50)))
        {
            buttonController->toggleLightSourceMovementBehaviour();
        }

        if (ImGui::IsItemHovered())
        {
            std::string text;

            switch (*lightSourceMovementBehaviour)
            {
            case LightSourceMovementBehaviour::FIXED_POSITION:
                text = "Fixed Position";
                break;
            case LightSourceMovementBehaviour::FOLLOW_CAMERA:
                text = "Follow Camera";
                break;
            }
            if (text.empty())
            {
                throw std::runtime_error("Invalid Light Source Movement Behaviour");
            }

            ImGui::SetTooltip("Toggle Light Source (L), Current: %s", text.c_str());
        }
        ImGui::Spacing();


        if (ImGui::ImageButton(currentCameraIcon.id, ImVec2(50, 50)))
        {
            buttonController->toggleCameraFocusMode();
        }

        if (ImGui::IsItemHovered())
        {
            std::string text;
            switch (*cameraMode)
            {
            case CameraFocusMode::CENTER_OF_MASS:
                text = "Center of Mass";
                break;
            case CameraFocusMode::FREE:
                text = "Free";
                break;

            default:
                throw std::runtime_error("Invalid Camera Focus Mode");
            }

            if (text.empty())
            {
                throw std::runtime_error("Invalid Camera Movement Behaviour");
            }


            ImGui::SetTooltip("Toggle Camera Movement (K), Current: %s", text.c_str());
        }


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
        float cylRadPreCalc = sqrt(renderer->cylinderRadius);
        float sphereRadPreCalc = sqrt(renderer->sphereRadius);

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

    void Buttons::scaleGraph()
    {
        static float scale = 1.0f;

        if (ImGui::BeginMenu("Scale Graph"))
        {
            if (ImGui::DragFloat("##Scale", &scale, 0.001f, 0.01f, 100.0f))
            {
                buttonController->scaleGraph(scale);
            }
            {
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
            ImGui::Separator();
            scaleGraph();


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
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_AlwaysVerticalScrollbar
        ))
        {
            if (ImGui::Button("Import Group Config"))
            {
                importGroupConfigBrowser.SetTitle("Import Group Config");
                importGroupConfigBrowser.Open();
            }
            ImGui::SameLine();
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
                        randomizeColoring(group.getID());
                        changeColoring(group.getID());

                        ChangeTransparency(group.getID());
                    }
                }
            }
            ImGui::EndPopup();
        } else {
            for (const auto& group : *activeGroups) {

                colorBuffer[group.getID()] = group.getVec4();

            }
        }
    }

    void Buttons::ChangeTransparency(uint32_t groupID)
    {
        float transparency = saver->getGraph().getGroupByID(groupID).getVec4().w;


        ImGui::Text("Transparency:");
        ImGui::SameLine();
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
                              ImGuiWindowFlags_NoCollapse ))
        {
            if (ImGui::Button("Remove Highlighting (H)"))
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
        static const char* modeText[] = {"Performance", "Balance", "Quality"};
        static bool is_toggled = false;

        if (ImGui::BeginPopup(popUpName,
                              ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoCollapse
        ))
        {
            if (ImGui::SliderInt("##ModeSlider", (int*)performanceMode,
                                 static_cast<int>(PerformanceMode::PERFORMANCE),
                                 static_cast<int>(PerformanceMode::QUALITY),
                                 modeText[static_cast<int>(*performanceMode)]))
            {
                buttonController->setPerformanceMode(*performanceMode);
            }
            if (ImGui::Checkbox("Use lines to represent edges", &is_toggled)) {
                Renderer::getInstance()->setUseCylindersForEdges(!is_toggled);
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

    void Buttons::changeColoring(uint32_t groupID) const
    {
        ImVec4 current_color = saver->getGraph().getGroupByID(groupID).getVec4();
        const ImVec4 *colorBuf = &colorBuffer.at(groupID);


        if (colorBuf->x == 0 && colorBuf->y == 0 && colorBuf->z == 0 && colorBuf->w == 0)
        {
            current_color = colorBuffer[groupID];
        }


        if (ImGui::ColorEdit3(std::format("##Change Color Edit{}", groupID).c_str(), &current_color.x))
        {
            buttonController->changeColoring(groupID, current_color);
        }
        ImGui::SameLine();
        if (ImGui::Button(std::format("Revert##{}", groupID).c_str()))
        {
            buttonController->changeColoring(groupID, *colorBuf);
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

            ImGui::Separator();

            if (ImGui::BeginMenu("Recently Opened"))
            {
                const auto filenames = CachingController::getCachedGraphFilenames();

                for (auto const& file : filenames)
                {
                    if (ImGui::MenuItem(file.stem().c_str()))
                    {
                        CachingController::loadCachedGraph(file);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Example Graphs"))
            {
                const auto filenames = CachingController::getExampleGraphNames();

                for (auto const& file : filenames)
                {
                    if (ImGui::MenuItem(file.stem().c_str()))
                    {
                        CachingController::loadCachedGraph(file);
                    }
                }

                ImGui::EndMenu();
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
                constexpr auto infoText = "This is a simple 3D graph viewer. \n"
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
            if (ImGui::BeginMenu("Controls"))
            {
                const auto controlsText =
                    "Free Camera Mode: \n"
                    "Move Up : Space Bar \n"
                    "Move down : Left Control  \n"
                    "Move left : A \n"
                    "Move right : D \n"
                    "Move forward : W \n"
                    "Move back : S \n"
                    "Sprint : Left Shift\n";

                ImGui::Text(controlsText);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Hotkeys"))
            {
                const auto hotkeyText = "- K : Change Camera Movement Mode \n"
                    "- Q : Rotate through Rendering Qualities \n"
                    "- L : Toggle Light Source Behavior \n"
                    "- H : Remove Highlighting";

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