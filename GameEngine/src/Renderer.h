#pragma once

#include <memory>
#include <array>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Scene.h"
#include "Window.h"

class Renderer {
public:
    Renderer(std::shared_ptr<Window> window);
    ~Renderer();

    void Init();
    void Render(Scene& scene);
    void RenderToViewport(Scene& scene, int viewportWidth, int viewportHeight); // Render for editor viewport
    void Shutdown();

    GLuint GetRenderTexture() const;

private:
    GLuint shadowFBO;
    GLuint shadowMap;
    GLuint renderFBO;
    GLuint renderTexture;
    GLuint rboDepth; // Depth buffer for FBO

    std::shared_ptr<Shader> m_PBRShader;
    std::shared_ptr<Shader> m_ShadowShader;
    std::shared_ptr<Shader> m_SkyboxShader;

    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Scene> m_Scene;

    void SetupLights();
    bool IsInFrustum(const glm::vec3& center, const glm::vec3& halfSize, const std::array<glm::vec4, 6>& planes);

    int m_ViewportWidth;  // Store the viewport width
    int m_ViewportHeight; // Store the viewport height
    void UpdateViewportSize(int width, int height);
};
