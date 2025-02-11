#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Editor.h"

class GameLayer;
class EditorLayer;

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

    GameLayer* m_GameLayer = nullptr;
    EditorLayer* m_EditorLayer = nullptr;

    bool m_PlayMode = false;
};
