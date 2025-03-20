#define GLEW_STATIC
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "Editor.h"
#include "MeshRenderer.h"
#include "EditorStyle.h"

#include "Application.h"
#include "glm/gtc/type_ptr.hpp"

// Function to draw a gradient rectangle
void DrawGradient(ImVec2 start, ImVec2 end, ImU32 colorStart, ImU32 colorEnd) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilledMultiColor(start, end, colorStart, colorStart, colorEnd, colorEnd);
}

void Editor::Init(GLFWwindow* window, Scene& scene, Renderer& renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Apply the custom style
    EditorStyle::ApplyStyle();

    io.IniFilename = "editor_layout.ini";

    // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Load a larger font
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Arial.ttf", 22.0f); // Change the path and size as needed

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    m_Window = window;
    m_Scene = &scene;
    m_Renderer = &renderer;

    // Create Framebuffer
    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    // Create Texture to render to
    glGenTextures(1, &m_TextureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer, 0);

    // Create Renderbuffer Object for depth and stencil attachment
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Check if framebuffer is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " << status << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::Render() {

    // Set up main DockSpace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 11.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // Show the main menu bar
    ShowMainMenuBar();

    ImGui::End();

 // Draw the two separate viewports.
    ShowEditorViewport();  // This always shows the scene for editing.
    ShowGameViewport();    // This shows the game simulation preview.

    // Draw the playback control button.
    ShowPlayStopButton();

    // Show other panels
    ShowSceneHierarchy();
    ShowInspector();
    ShowProject();
    ShowConsole();

    // Disable depth testing before rendering ImGui
    glDisable(GL_DEPTH_TEST);

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void Editor::Shutdown() {

    ImGui::SaveIniSettingsToDisk("editor_layout.ini");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup framebuffer
    glDeleteFramebuffers(1, &m_Framebuffer);
    glDeleteTextures(1, &m_TextureColorbuffer);
    glDeleteRenderbuffers(1, &m_RBO);
}

void Editor::ShowMainMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 15)); // Increase height of the menu bar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.96f, 0.98f, 1.00f)); // Text color

    ImGui::BeginMainMenuBar();

    // Draw the gradient background
    ImVec2 menuBarMin = ImGui::GetWindowPos();
    ImVec2 menuBarMax = ImVec2(menuBarMin.x + ImGui::GetWindowWidth(), menuBarMin.y + ImGui::GetWindowHeight());

    float gradientWidth = menuBarMax.x * 0.30f; // 25% width for the gradient
    ImVec2 gradientEnd = ImVec2(menuBarMin.x + gradientWidth, menuBarMax.y);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilledMultiColor(menuBarMin, gradientEnd, ImColor(14, 105, 113, 255), ImColor(80, 80, 80, 255), ImColor(80, 80, 80, 255), ImColor(14, 105, 113, 255));

    drawList->AddRectFilled(ImVec2(gradientEnd.x, menuBarMin.y), menuBarMax, ImColor(80, 80, 80, 255));

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New")) {
            // Handle new scene
        }
        if (ImGui::MenuItem("Open...")) {
            // Handle open scene
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            // Handle save scene
        }
        if (ImGui::MenuItem("Save As...")) {
            // Handle save scene as
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit")) {
            // Handle exit
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            // Handle undo
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            // Handle redo
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "Ctrl+X")) {
            // Handle cut
        }
        if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            // Handle copy
        }
        if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            // Handle paste
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void Editor::ShowSceneHierarchy() {
    ImGui::Begin("Hierarchy");
    for (const auto& gameObject : m_Scene->GetGameObjects()) {
        ShowGameObjectHierarchy(gameObject.get());
    }
    ImGui::End();
}

void Editor::ShowGameObjectHierarchy(GameObject* gameObject) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (gameObject == m_SelectedGameObject) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (gameObject->GetChildren().empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, gameObject->GetName().c_str());

    if (ImGui::IsItemClicked()) {
        m_SelectedGameObject = gameObject;
    }

    if (nodeOpen) {
        for (const auto& child : gameObject->GetChildren()) {
            ShowGameObjectHierarchy(child.get());
        }
        ImGui::TreePop();
    }
}

void Editor::ShowInspector() {
    ImGui::Begin("Inspector");

    if (m_SelectedGameObject) {
        // Display the name of the selected GameObject.
        ImGui::Text("Selected: %s", m_SelectedGameObject->GetName().c_str());

        // (Display Transform data here, as before)
        Transform* transform = m_SelectedGameObject->GetTransform();
        if (transform) {
            if (ImGui::CollapsingHeader("TRANSFORM", ImGuiTreeNodeFlags_DefaultOpen)) {
                // A lambda for drawing transform controls (Position, Rotation, Scale)
                auto DrawTransformControl = [&](const char* label, glm::vec3& values, const char* id) {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 100);
                    ImGui::Text("%s", label);
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(80);
                    ImGui::TextColored(ImVec4(0.8f,0.0f,0.0f,1.0f), "X");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##X") + id).c_str(), &values.x, 0.1f);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f,0.8f,0.0f,1.0f), "Y");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##Y") + id).c_str(), &values.y, 0.1f);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f,0.0f,0.8f,1.0f), "Z");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##Z") + id).c_str(), &values.z, 0.1f);
                    ImGui::PopItemWidth();
                    ImGui::Columns(1);
                };

                glm::vec3 pos = transform->GetPosition();
                DrawTransformControl("Position", pos, "Pos");
                transform->SetPosition(pos);

                glm::vec3 rot = transform->GetRotation();
                DrawTransformControl("Rotation", rot, "Rot");
                transform->SetRotation(rot);

                glm::vec3 scale = transform->GetScale();
                DrawTransformControl("Scale", scale, "Scale");
                transform->SetScale(scale);
            }
        }

        // Material Editing Section for MeshRenderer
        MeshRenderer* meshRenderer = m_SelectedGameObject->GetComponent<MeshRenderer>();
        if (meshRenderer) {
            if (ImGui::CollapsingHeader("MESH RENDERER", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Display the material name.
                if (meshRenderer->GetMaterial()) {
                    ImGui::Text("Material: %s", meshRenderer->GetMaterial()->GetName().c_str());
                }

                // Show Material Properties (Base Color, Metallic, Roughness, and Texture Previews)
                if (meshRenderer->GetMaterial()) {
                    if (ImGui::CollapsingHeader("MATERIAL PROPERTIES", ImGuiTreeNodeFlags_DefaultOpen)) {
                        std::shared_ptr<Material> material = meshRenderer->GetMaterial();

                        ImGui::Separator();
                        // Base Color
                        {
                            glm::vec3 baseColor = material->GetBaseColor();
                            ImGui::Text("Base Color");
                            ImGui::SameLine(120);
                            if (ImGui::ColorEdit3("##BaseColor", glm::value_ptr(baseColor))) {
                                material->SetBaseColor(baseColor);
                            }
                        }

                        // Metallic
                        {
                            float metallic = material->GetMetallic();
                            ImGui::Text("Metallic");
                            ImGui::SameLine(120);
                            if (ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f)) {
                                material->SetMetallic(metallic);
                            }
                        }

                        // Roughness
                        {
                            float roughness = material->GetRoughness();
                            ImGui::Text("Roughness");
                            ImGui::SameLine(120);
                            if (ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f)) {
                                material->SetRoughness(roughness);
                            }
                        }

                        ImGui::Separator();
                        ImGui::Text("Texture Previews:");

                        // Use a table layout for texture previews.
                        if (ImGui::BeginTable("MaterialTextures", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Albedo");
                            ImGui::TableSetColumnIndex(1);
                            auto albedoTexture = material->GetAlbedoMap();
                            if (albedoTexture) {
                                ImGui::Image((void*)(intptr_t)albedoTexture->GetID(), ImVec2(50,50));
                            } else {
                                ImGui::Text("None");
                            }

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Metallic");
                            ImGui::TableSetColumnIndex(1);
                            auto metallicTexture = material->GetMetallicMap();
                            if (metallicTexture) {
                                ImGui::Image((void*)(intptr_t)metallicTexture->GetID(), ImVec2(50,50));
                            } else {
                                ImGui::Text("None");
                            }

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Roughness");
                            ImGui::TableSetColumnIndex(1);
                            auto roughnessTexture = material->GetRoughnessMap();
                            if (roughnessTexture) {
                                ImGui::Image((void*)(intptr_t)roughnessTexture->GetID(), ImVec2(50,50));
                            } else {
                                ImGui::Text("None");
                            }

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Normal");
                            ImGui::TableSetColumnIndex(1);
                            auto normalTexture = material->GetNormalMap();
                            if (normalTexture) {
                                ImGui::Image((void*)(intptr_t)normalTexture->GetID(), ImVec2(50,50));
                            } else {
                                ImGui::Text("None");
                            }
                            ImGui::EndTable();
                        }
                    }
                }
            }
        }
    } else {
        ImGui::Text("No object selected");
    }
    ImGui::End();
}

void Editor::ShowProject() {
    ImGui::Begin("Project");
    // Show the project files and folders here
    ImGui::Text("Assets");
    ImGui::Text("Scenes");
    ImGui::End();
}

void Editor::ShowConsole() {
    ImGui::Begin("Console");
    // Show console output here
    ImGui::Text("Console output");
    ImGui::End();
}

void Editor::ShowEditorViewport() {
    ImGui::Begin("Editor Viewport");

    // Get the available size in this window.
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Render the scene to the editor view.
    // You might choose to use a different render mode or overlays here.
    m_Renderer->RenderToViewport(*m_Scene, static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));

    // Retrieve the render texture and display it.
    GLuint renderTexture = m_Renderer->GetRenderTexture();
    ImGui::Image((void*)(intptr_t)renderTexture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void Editor::ShowGameViewport() {
    ImGui::Begin("Game View");

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Render the scene as the game would see it.
    // (This could use a separate camera or post-processing effects.)
    m_Renderer->RenderToViewport(*m_Scene, static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));

    GLuint gameTexture = m_Renderer->GetRenderTexture(); // You may use the same or a separate target.
    ImGui::Image((void*)(intptr_t)gameTexture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void Editor::ShowPlayStopButton() {
    // Begin a child region for playback control (for better layout).
    ImGui::BeginChild("PlaybackControl", ImVec2(0, 60), false, ImGuiWindowFlags_NoScrollbar);

    const ImVec2 buttonSize(100, 50);
    // Determine the label based on the play mode state.
    std::string label = "Play";
    if (m_App && m_App->IsPlayMode()) {
        label = "Stop";
    }

    // Center the button.
    float windowWidth = ImGui::GetWindowWidth();
    float offsetX = (windowWidth > buttonSize.x) ? (windowWidth - buttonSize.x) * 0.5f : 0.0f;
    ImGui::SetCursorPosX(offsetX);

    if (ImGui::Button(label.c_str(), buttonSize)) {
        if (m_App) {
            // Toggle the play mode.
            m_App->SetPlayMode(!m_App->IsPlayMode());
        }
    }

    ImGui::EndChild();
}


