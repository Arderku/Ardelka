#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Editor.h"

class Engine {
public:
    void Run();
    Scene& GetScene() { return m_Scene; }

    void Init();

    Renderer GetRenderer();

private:
    std::shared_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    Scene m_Scene;
    Editor m_Editor;
};
