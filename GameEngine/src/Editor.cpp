#define GLEW_STATIC
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "Editor.h"
#include "MeshRenderer.h"
#include "EditorStyle.h"

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
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

    // Show the scene viewport
    ShowSceneViewport();

    // Show other panels
    ShowSceneHierarchy();
    ShowInspector();
    ShowProject();
    ShowConsole();

    // Disable depth testing before rendering ImGui
    glDisable(GL_DEPTH_TEST);

    // Render ImGui elements
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void Editor::Shutdown() {
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
        ImGui::Text(m_SelectedGameObject->GetName().c_str());

        Transform* transform = m_SelectedGameObject->GetTransform();
        if (transform) {
            if (ImGui::CollapsingHeader("TRANSFORM", ImGuiTreeNodeFlags_DefaultOpen)) {

                ImVec4 colorX = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
                ImVec4 colorY = ImVec4(0.0f, 0.8f, 0.0f, 1.0f);
                ImVec4 colorZ = ImVec4(0.0f, 0.0f, 0.8f, 1.0f);

                auto DrawTransformControl = [&](const char* label, glm::vec3& values, const char* id) {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 100);
                    ImGui::Text(label);
                    ImGui::NextColumn();

                    ImGui::PushItemWidth(80);

                    ImGui::TextColored(colorX, "X");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##X") + id).c_str(), &values.x, 0.1f);

                    ImGui::SameLine();

                    ImGui::TextColored(colorY, "Y");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##Y") + id).c_str(), &values.y, 0.1f);

                    ImGui::SameLine();

                    ImGui::TextColored(colorZ, "Z");
                    ImGui::SameLine();
                    ImGui::DragFloat((std::string("##Z") + id).c_str(), &values.z, 0.1f);

                    ImGui::Columns(1);
                };

                glm::vec3 position = transform->GetPosition();
                DrawTransformControl("Position", position, "Position");
                transform->SetPosition(position);

                glm::vec3 rotation = transform->GetRotation();
                DrawTransformControl("Rotation", rotation, "Rotation");
                transform->SetRotation(rotation);

                glm::vec3 scale = transform->GetScale();
                DrawTransformControl("Scale", scale, "Scale");
                transform->SetScale(scale);
            }
        }

        // Iterate through components and show MeshRenderer if present
        MeshRenderer* meshRenderer = m_SelectedGameObject->GetComponent<MeshRenderer>();
        if (meshRenderer) {
            if (ImGui::CollapsingHeader("MESH RENDERER", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (meshRenderer->GetMaterial()) {
                    ImGui::Text("Material: %s", meshRenderer->GetMaterial()->GetName().c_str());
                }

                if (meshRenderer->GetMaterial()) {
                    if (ImGui::CollapsingHeader("MATERIAL", ImGuiTreeNodeFlags_DefaultOpen)) {

                        // Display controls for BaseColor, Metallic, and Roughness
                        glm::vec3 baseColor = meshRenderer->GetMaterial()->GetBaseColor();
                        if (ImGui::ColorEdit3("Base Color", &baseColor[0])) {
                            meshRenderer->GetMaterial()->SetBaseColor(baseColor);
                        }

                        float metallic = meshRenderer->GetMaterial()->GetMetallic();
                        if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f)) {
                            meshRenderer->GetMaterial()->SetMetallic(metallic);
                        }

                        float roughness = meshRenderer->GetMaterial()->GetRoughness();
                        if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
                            meshRenderer->GetMaterial()->SetRoughness(roughness);
                        }

                        // Textures preview
                        ImVec2 textureSize(50, 50);
                        // Base color
                        auto albedoTexture = meshRenderer->GetMaterial()->GetAlbedoMap();
                        if (albedoTexture) {
                            // Display texture preview
                            ImGui::Text("Albedo:");
                            ImGui::Image((void *)(intptr_t) albedoTexture->GetID(), textureSize);
                        } else {
                            ImGui::Text("Albedo: No texture");
                            // empty texture
                            ImGui::Image((void *)(intptr_t) nullptr, textureSize);
                        }

                        auto metallicTexture = meshRenderer->GetMaterial()->GetMetallicMap();
                        if (metallicTexture) {
                            // Display texture preview
                            ImGui::Text("Metallic:");
                            ImGui::Image((void *)(intptr_t) metallicTexture->GetID(), textureSize);
                        } else {
                            ImGui::Text("Metallic: No texture");
                            // empty texture
                            ImGui::Image((void *)(intptr_t) nullptr, textureSize);
                        }

                        auto roughnessTexture = meshRenderer->GetMaterial()->GetRoughnessMap();
                        if (roughnessTexture) {
                            // Display texture preview
                            ImGui::Text("Roughness:");
                            ImGui::Image((void *)(intptr_t) roughnessTexture->GetID(), textureSize);
                        } else {
                            ImGui::Text("Roughness: No texture");
                            // empty texture
                            ImGui::Image((void *)(intptr_t) nullptr, textureSize);
                        }

                        auto normalTexture = meshRenderer->GetMaterial()->GetNormalMap();
                        if (normalTexture) {
                            // Display texture preview
                            ImGui::Text("Normal:");
                            ImGui::Image((void *)(intptr_t) normalTexture->GetID(), textureSize);
                        } else {
                            ImGui::Text("Normal: No texture");
                            // empty texture
                            ImGui::Image((void *)(intptr_t) nullptr, textureSize);
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

void Editor::ShowSceneViewport() {
    ImGui::Begin("Scene Viewport");

    // Get the size of the viewport window
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Update the framebuffer size to match the viewport size only if it changes
    static ImVec2 lastViewportSize = ImVec2(0, 0);
    if (viewportSize.x != lastViewportSize.x || viewportSize.y != lastViewportSize.y) {
        glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)viewportSize.x, (int)viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)viewportSize.x, (int)viewportSize.y);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        lastViewportSize = viewportSize;

        // Update the camera's projection matrix
        float aspectRatio = viewportSize.x / viewportSize.y;
        m_Scene->GetActiveCamera()->SetAspectRatio(aspectRatio);
    }

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glViewport(0, 0, (int)viewportSize.x, (int)viewportSize.y);

    // Clear the framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene to the framebuffer
    if (m_Renderer && m_Scene) {
        m_Renderer->RenderToFramebuffer(*m_Scene, m_Framebuffer, (int)viewportSize.x, (int)viewportSize.y);
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Display the framebuffer texture in the ImGui window
    ImGui::Image((void*)(intptr_t)m_TextureColorbuffer, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    // Disable depth testing before rendering ImGui elements
    glDisable(GL_DEPTH_TEST);

    // Render text on top of the scene
    ImGui::SetCursorPos(ImVec2(50, 60)); // Adjust the position as needed
    ImGui::Text("STATS:");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); // Display FPS
    ImGui::Text("Viewport Size: (%.0f, %.0f)", viewportSize.x, viewportSize.y); // Display viewport size
    ImGui::Text("Aspect Ratio: %.2f", viewportSize.x / viewportSize.y);

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);

    ImGui::End();
}


void Editor::ShowPlayPauseStopButtons() {
    // Calculate the position to center the buttons at the top
    ImVec2 buttonSize(50, 50); // Adjust the size of the buttons
    float totalWidth = buttonSize.x * 3 + ImGui::GetStyle().ItemSpacing.x * 2; // 3 buttons with spacing
    ImVec2 buttonPos((800 - totalWidth) / 2, 50.0f); // 10.0f for top margin

    ImGui::SetCursorPos(buttonPos);

    if (ImGui::Button("Play", buttonSize)) {
        // Handle play action
    }

    ImGui::SameLine();
    if (ImGui::Button("Pause", buttonSize)) {
        // Handle pause action
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop", buttonSize)) {
        // Handle stop action
    }
}
