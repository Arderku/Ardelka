#define GLEW_STATIC
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "Editor.h"

// Function to draw a gradient rectangle
void DrawGradient(ImVec2 start, ImVec2 end, ImU32 colorStart, ImU32 colorEnd) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilledMultiColor(start, end, colorStart, colorStart, colorEnd, colorEnd);
}

void Editor::Init(GLFWwindow* window, Scene& scene, Renderer& renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Customize the style based on the provided image colors
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.FramePadding = ImVec2(8, 10);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(10, 10);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.1686f, 0.1765f, 0.1882f, 1.00f); // Dark background
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.27f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.34f, 0.35f, 0.36f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.41f, 0.42f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.29f, 0.39f, 0.85f, 1.00f); // Bright blue
    colors[ImGuiCol_SliderGrab] = ImVec4(0.29f, 0.39f, 0.85f, 1.00f); // Bright blue
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.49f, 0.95f, 1.00f); // Brighter blue
    colors[ImGuiCol_Button] = ImVec4(0.29f, 0.39f, 0.85f, 1.00f); // Bright blue
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.49f, 0.95f, 1.00f); // Brighter blue
    colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.59f, 1.00f, 1.00f); // Brightest blue
    colors[ImGuiCol_Header] = ImVec4(0.29f, 0.39f, 0.85f, 1.00f); // Bright blue
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.39f, 0.49f, 0.95f, 1.00f); // Brighter blue
    colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.59f, 1.00f, 1.00f); // Brightest blue
    colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.34f, 0.35f, 0.36f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.39f, 0.85f, 1.00f); // Bright blue
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.39f, 0.49f, 0.95f, 1.00f); // Brighter blue
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.59f, 1.00f, 1.00f); // Brightest blue
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.1686f, 0.1765f, 0.1882f, 1.00f); // Bright blue
    colors[ImGuiCol_TabActive] = ImVec4(0.1686f, 0.1765f, 0.1882f, 1.00f); // Brighter blue
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1686f, 0.1765f, 0.1882f, 1.00f); // Bright blue
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.29f, 0.39f, 0.85f, 0.35f); // Bright blue
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.29f, 0.39f, 0.85f, 0.95f); // Bright blue
    colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

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
        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is undefined!" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer incomplete attachment!" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer missing attachment!" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer incomplete draw buffer!" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer incomplete read buffer!" << std::endl;
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer unsupported!" << std::endl;
                break;
            default:
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer incomplete for unknown reason!" << std::endl;
                break;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::Render() {
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

    // Show the scene hierarchy panel
    ShowSceneHierarchy();

    // Show the inspector panel
    ShowInspector();

    // Show the project panel
    ShowProject();

    // Show the console panel
    ShowConsole();

    // Show play, pause, stop buttons
   // ShowPlayPauseStopButtons();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    drawList->AddRectFilledMultiColor(menuBarMin, gradientEnd, ImColor(103, 58, 183, 255), ImColor(80, 80, 80, 255), ImColor(80, 80, 80, 255), ImColor(103, 58, 183, 255));
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
    // Show the list of objects in the scene here
    ImGui::Text("Object 1");
    ImGui::Text("Object 2");
    ImGui::End();
}

void Editor::ShowInspector() {
    ImGui::Begin("Inspector");
    // Show details of the selected object here
    ImGui::Text("Inspector details for selected object");
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

    // Update the framebuffer size to match the viewport size
    glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)viewportSize.x, (int)viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)viewportSize.x, (int)viewportSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

    // Calculate FPS
    static float frameTime = 0.0f;
    static int frameCount = 0;
    frameCount++;
    frameTime += ImGui::GetIO().DeltaTime;
    if (frameTime >= 1.0f) {
        frameCount = 0;
        frameTime = 0.0f;
    }
    float fps = 1.0f / ImGui::GetIO().DeltaTime;

    // Set the position for the FPS counter
    ImVec2 fpsPosition = ImVec2(viewportSize.x - 60.0f, 10.0f); // Adjust the position as needed
    ImGui::SetCursorPos(fpsPosition);

    // Set the text color to purple
    ImVec4 purpleColor = ImVec4(0.5f, 0.0f, 0.5f, 1.0f); // RGBA
    ImGui::PushStyleColor(ImGuiCol_Text, purpleColor);
    ImGui::Text("FPS: %.0f", fps);
    //log fps
    std::cout << "FPS: " << fps << std::endl;
    ImGui::PopStyleColor();

    // Show Play, Pause, Stop buttons in the center top
    ShowPlayPauseStopButtons();

    ImGui::End();
}

void Editor::ShowPlayPauseStopButtons() {
    // Calculate the position to center the buttons at the top
    ImVec2 buttonSize(50, 30); // Adjust the size of the buttons
    float totalWidth = buttonSize.x * 3 + ImGui::GetStyle().ItemSpacing.x * 2; // 3 buttons with spacing
    ImVec2 buttonPos((800 - totalWidth) / 2, 10.0f); // 10.0f for top margin

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
