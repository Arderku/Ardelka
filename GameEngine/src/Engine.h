#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Scripting.h"
#include "Editor.h"

class Engine {
public:
    void Run();
    Scene& GetScene() { return m_Scene; }

    void Init();

    Renderer GetRenderer();

private:
    Window m_Window;
    Renderer m_Renderer;
    Scene m_Scene;
    Scripting m_Scripting;
    Editor m_Editor;
};
