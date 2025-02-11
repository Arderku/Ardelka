#define GLEW_STATIC
#include <GL/glew.h>
#include <chrono>
#include "Application.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Logger.h"

#include "GameLayer.h"
#include "EditorLayer.h"

void Application::Run() {
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();
    int frameCount = 0;

    while (m_Window->IsOpen()) {
        auto currentTime = clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;
        float deltaTime = elapsed.count();
        frameCount++;

        // Calculate and print FPS every second
        static float timeAccumulator = 0.0f;
        timeAccumulator += elapsed.count();
        if (timeAccumulator >= 1.0f) {
            float fps = frameCount / timeAccumulator;
            Logger::Log("FPS: " + std::to_string(fps));
            frameCount = 0;
            timeAccumulator = 0.0f;
        }

        m_Window->PollEvents();

        if (m_PlayMode) {
            m_GameLayer->OnUpdate(deltaTime);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_EditorLayer->OnImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_Window->SwapBuffers();

    }

    m_Renderer->Shutdown();
    m_EditorLayer->OnDetach();
}

void Application::Init() {
    m_Window = std::make_shared<Window>();
    m_Window->Create(1200, 600, "Game Engine");

    m_Renderer = std::make_unique<Renderer>(m_Window);
    m_Renderer->Init();

    m_Editor.Init(m_Window->GetWindow(), m_Scene, *m_Renderer);
    m_Editor.SetApplication(this);

    m_GameLayer = new GameLayer(m_Scene, *m_Renderer);

    m_EditorLayer = new EditorLayer(m_Editor);
}

Renderer Application::GetRenderer() {
    return *m_Renderer;
}
