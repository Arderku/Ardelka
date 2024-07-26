#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Scene.h"

class Editor {
public:
    void Init(GLFWwindow* window, Scene& scene, Renderer& renderer);
    void Render();
    void Shutdown();

private:
    void ShowMainMenuBar();
    void ShowSceneHierarchy();
    void ShowInspector();
    void ShowProject();
    void ShowConsole();
    void ShowSceneViewport();

    GLuint m_Framebuffer;
    GLuint m_TextureColorbuffer;
    GLuint m_RBO;

    Scene* m_Scene;  // Pointer to the scene
    Renderer* m_Renderer;  // Pointer to the renderer
    GLFWwindow* m_Window;  // Store a pointer to the GLFW window

    void ShowPlayPauseStopButtons();

    void ShowGameObjectHierarchy(GameObject *gameObject);

    GameObject *m_SelectedGameObject = nullptr;
};
