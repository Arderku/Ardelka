#define GLEW_STATIC
#include <GL/glew.h>
#include "Engine.h"

void Engine::Run() {


    while (m_Window.IsOpen()) {
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
