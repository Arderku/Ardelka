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

    m_Shader = new Shader("resources/shaders/vertex_shader.glsl", "resources/shaders/fragment_shader.glsl");
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

    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 2.0f, -5.0f), // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f),   // Look at origin
            glm::vec3(0.0f, 1.0f, 0.0f)    // Up vector
    );

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    m_Shader->setMat4("view", view);
    m_Shader->setMat4("projection", projection);

    // Set directional light
    m_Shader->setDirectionalLight("dirLight", dirLight);

    // Set point lights
    int numPointLights = 4; // Adjust this as needed
    m_Shader->setInt("numPointLights", numPointLights);
    for (int i = 0; i < numPointLights; ++i) {
        std::string uniformName = "pointLights[" + std::to_string(i) + "]";
        m_Shader->setPointLight(uniformName, pointLights[i]);
        //random light pos
        pointLights[i].position = glm::vec3(sin(glfwGetTime()) * 2.0f *i, 0.0f, cos(glfwGetTime()) * 2.0f +i);
    }

    glm::vec3 viewPos = glm::vec3(0.0f, 8.0f, -10.0f); // Adjusted to match the camera position in the view matrix
    m_Shader->setVec3("viewPos", viewPos);

   scene.Render();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after rendering plane: " << error << std::endl;
    }

    m_Shader->unuse();
    glUseProgram(0);
}

void Renderer::RenderToFramebuffer(Scene& scene, GLuint framebuffer, int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    Render(scene);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Shutdown() {
    delete m_Shader;

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

Shader * Renderer::GetShader() {
    return m_Shader;
}
