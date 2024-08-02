#pragma once

#include "Scene.h"
#include "Light.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Renderer {
public:
    void Init();
    void Render(Scene& scene);
    void RenderToFramebuffer(Scene& scene, GLuint framebuffer, int width, int height);
    void Shutdown();

private:
    GLuint m_VAO, m_VBO, m_EBO;
    glm::vec3 m_LightPos;
    glm::vec3 m_SunDirection;
    glm::vec3 m_SunColor;

    DirectionalLight dirLight;
    PointLight pointLights[4];

    bool IsInFrustum(const glm::vec3& center, const glm::vec3& halfSize, const std::array<glm::vec4, 6>& planes);
};
