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
    void RenderToFramebuffer(Scene& scene, GLuint framebuffer, int width, int height);
    void Shutdown();

private:
    GLuint shadowFBO;
    GLuint shadowMap;

    std::shared_ptr<Shader> m_PBRShader;
    std::shared_ptr<Shader> m_ShadowShader;
    std::shared_ptr<Shader> m_SkyboxShader;

    std::shared_ptr<Window> m_Window;

    void SetupLights();
    bool IsInFrustum(const glm::vec3& center, const glm::vec3& halfSize, const std::array<glm::vec4, 6>& planes);
};
