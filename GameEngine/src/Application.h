#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Editor.h"
#include <memory>
#include "EditorLayer.h"
#include "GameLayer.h"

class Application {
public:
    void Init();
    void Run();
    Scene& GetScene() { return m_Scene; }
    Renderer GetRenderer();

    bool IsPlayMode() const { return m_PlayMode; }
    void SetPlayMode(bool playMode) { m_PlayMode = playMode; }
    void TogglePlayMode() { m_PlayMode = !m_PlayMode; }

private:
    std::shared_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    Scene m_Scene;
    Editor m_Editor;

    std::unique_ptr<GameLayer> m_GameLayer;
    std::unique_ptr<EditorLayer> m_EditorLayer;

    bool m_PlayMode = false;
};
