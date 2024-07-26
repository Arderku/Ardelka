#define GLEW_STATIC
#include <GL/glew.h>
#include <chrono>
#include "Engine.h"
#include "Logger.h"

void Engine::Run() {
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();
    int frameCount = 0;

    while (m_Window.IsOpen()) {
        auto currentTime = clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;
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

        m_Window.PollEvents();
        m_Scene.Update();

        // m_Scripting.ExecuteScripts(m_Scene);
        m_Renderer.Render(m_Scene);
        m_Editor.Render();
        m_Window.SwapBuffers();
    }

    m_Renderer.Shutdown();
    // m_Scripting.Shutdown();
    m_Editor.Shutdown();
}

void Engine::Init() {
    m_Window.Create(1400, 900, "Ardelka Game Engine");
    m_Renderer.Init();
    // m_Scripting.Init();
    m_Editor.Init(m_Window.GetWindow(), m_Scene, m_Renderer);
}

Renderer Engine::GetRenderer() {
    return m_Renderer;
}
