#pragma once

#include "Shader.h"
#include "Material.h"
#include "Scene.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class Renderer {
public:
    void Init();
    void Render(Scene& scene);
    void RenderToFramebuffer(Scene& scene, GLuint framebuffer, int width, int height);
    void Shutdown();

    Shader * GetShader();

private:
    Shader* m_Shader = nullptr;

    GLuint m_VAO, m_VBO, m_EBO;
    glm::vec3 m_LightPos;
    glm::vec3 m_SunDirection;
    glm::vec3 m_SunColor;

    DirectionalLight dirLight;
    PointLight pointLights[4];
};
