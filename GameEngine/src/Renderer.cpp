#include "Renderer.h"
#include "Shader.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "MeshRenderer.h"

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

    dirLight.direction = glm::vec3(0.f, 1.0f, 0.0f);
    dirLight.color = glm::vec3(1.0f, 0.9f, 0.8f);

    for (int i = 0; i < 4; ++i) {
        pointLights[i].position = glm::vec3(i * 2.0f, 3.0f, 2.0f);
        pointLights[i].color = glm::vec3(1.0f - i * 0.25f, i * 0.25f, 0.0f);
        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09f;
        pointLights[i].quadratic = 0.032f;
    }

    std::cout << "Renderer initialized successfully" << std::endl;
}

void Renderer::Render(Scene& scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* activeCamera = scene.GetActiveCamera();
    if (!activeCamera) {
        std::cerr << "No active camera set in the scene!" << std::endl;
        return;
    }

    glm::mat4 view = activeCamera->GetView();
    glm::mat4 projection = activeCamera->GetProjection();

    const auto& gameObjects = scene.GetGameObjects();
    for (const auto& gameObject : gameObjects) {
        if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
            Shader* shader = renderer->GetShader();
            shader->use();
            shader->setMat4("view", view);
            shader->setMat4("projection", projection);

            // Ensure model matrix is set correctly if using it
            glm::mat4 model = gameObject->GetTransform()->GetModelMatrix();
            shader->setMat4("model", model);

            // Set other shader uniforms like lights if necessary

            renderer->Render();
            shader->unuse();
        }
    }

#ifdef DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after rendering: " << error << std::endl;
    }
#endif
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
    // Deleting VAO, VBO, and EBO only if they were initialized
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
}
