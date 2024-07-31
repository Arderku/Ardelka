#define GLM_ENABLE_EXPERIMENTAL
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "GLFW/glfw3.h"
#include "glm/gtx/string_cast.hpp"
#include "Mesh.h"

void Renderer::Init() {
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glClearColor(0.68f, 0.85f, 0.9f, 1.0f);

    m_Shader = new Shader("Resources/Shaders/vertex_shader.glsl", "Resources/Shaders/fragment_shader.glsl");
    if (!m_Shader->IsValid()) {
        std::cerr << "Failed to create Shader" << std::endl;
        return;
    }

    dirLight.direction = glm::vec3(0.f, 1.0f, 0.0f);
    dirLight.color = glm::vec3(1.0f, 0.9f, 0.8f);

    pointLights[0].position = glm::vec3(-2.0f, 3.0f, 2.0f);
    pointLights[0].color = glm::vec3(1.0f, 0.0f, 0.0f); // Red
    pointLights[0].constant = 1.0f;
    pointLights[0].linear = 0.09f;
    pointLights[0].quadratic = 0.032f;

    pointLights[1].position = glm::vec3(2.0f, 3.0f, 2.0f);
    pointLights[1].color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
    pointLights[1].constant = 1.0f;
    pointLights[1].linear = 0.09f;
    pointLights[1].quadratic = 0.032f;

    pointLights[2].position = glm::vec3(0.0f, 3.0f, -2.0f);
    pointLights[2].color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
    pointLights[2].constant = 1.0f;
    pointLights[2].linear = 0.09f;
    pointLights[2].quadratic = 0.032f;

    pointLights[3].position = glm::vec3(0.0f, 5.0f, 0.0f);
    pointLights[3].color = glm::vec3(1.0f, 1.0f, 1.0f); // White
    pointLights[3].constant = 1.0f;
    pointLights[3].linear = 0.09f;
    pointLights[3].quadratic = 0.032f;

    std::cout << "Renderer initialized successfully" << std::endl;
}

void Renderer::Render(Scene& scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->use();

    m_Shader->setMat4("view", scene.GetActiveCamera()->GetView());
    m_Shader->setMat4("projection", scene.GetActiveCamera()->GetProjection());

    // Set directional light
    m_Shader->setDirectionalLight("dirLight", dirLight);

    // Set point lights
    int numPointLights = 4; // Adjust this as needed
    m_Shader->setInt("numPointLights", numPointLights);
    for (int i = 0; i < numPointLights; ++i) {
        std::string uniformName = "pointLights[" + std::to_string(i) + "]";
        m_Shader->setPointLight(uniformName, pointLights[i]);

        // Update light positions only if necessary
        pointLights[i].position = glm::vec3(sin(glfwGetTime()) * 2.0f * i, 0.0f, cos(glfwGetTime()) * 2.0f + i);
    }

    m_Shader->setVec3("viewPos", scene.GetActiveCamera()->GetOwner()->GetTransform()->GetPosition());

    // Frustum culling
    const auto& frustumPlanes = scene.GetActiveCamera()->GetFrustumPlanes();
    for (auto& gameObject : scene.GetGameObjects()) {
        const glm::vec3& position = gameObject->GetTransform()->GetPosition();
        const glm::vec3& scale = gameObject->GetTransform()->GetScale();
        glm::vec3 halfSize = scale * 0.5f;

        if (IsInFrustum(position, halfSize, frustumPlanes)) {
            gameObject->Render();
        }
    }

#ifdef DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after rendering: " << error << std::endl;
    }
#endif

    m_Shader->unuse();
    glUseProgram(0);
}

void Renderer::RenderToFramebuffer(Scene& scene, GLuint framebuffer, int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    Render(scene);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Renderer::IsInFrustum(const glm::vec3& center, const glm::vec3& halfSize, const std::array<glm::vec4, 6>& planes) {
    for (const auto& plane : planes) {
        glm::vec3 normal = glm::vec3(plane);
        float distance = glm::dot(normal, center) + plane.w;
        float radius = glm::dot(halfSize, glm::abs(normal));

        if (distance + radius < 0) {
            return false;
        }
    }
    return true;
}

void Renderer::Shutdown() {
    delete m_Shader;

    // Deleting VAO, VBO, and EBO only if they were initialized
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

Shader* Renderer::GetShader() {
    return m_Shader;
}
