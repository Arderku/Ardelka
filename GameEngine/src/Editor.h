#pragma once

#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Scene.h"

class Application;

class Editor {
public:
    void Init(GLFWwindow* window, Scene& scene, Renderer& renderer);
    void Render();
    void Shutdown();

    void ShowEditorViewport(); // For editing the scene.
    void ShowGameViewport();   // For previewing the game simulation.

    void SetApplication(Application* app) { m_App = app; }

private:
    void ShowMainMenuBar();
    void ShowSceneHierarchy();
    void ShowInspector();
    void ShowProject();
    void ShowConsole();
   // void ShowSceneViewport();

    GLuint m_Framebuffer;
    GLuint m_TextureColorbuffer;
    GLuint m_RBO;

    Scene* m_Scene;  // Pointer to the scene
    Renderer* m_Renderer;  // Pointer to the renderer
    GLFWwindow* m_Window;  // Store a pointer to the GLFW window

    void ShowPlayStopButton();

    void ShowGameObjectHierarchy(GameObject *gameObject);

    GameObject *m_SelectedGameObject = nullptr;

    Application* m_App = nullptr;
};
