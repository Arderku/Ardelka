#include "Renderer.h"
#include "ResourceManager.h"
#include "MeshRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(std::shared_ptr<Window> window) : shadowFBO(0), shadowMap(0), m_Window(window) {
    // Constructor implementation
}

Renderer::~Renderer() {
    // Destructor implementation
    Shutdown();
}

void Renderer::Init() {
    // Initialize GLEW and OpenGL settings
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

    // Setup shadow map FBO
    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Load shaders
    m_PBRShader = ResourceManager::loadShader("PBR", "Resources/Shaders/pbr_vertex.glsl", "Resources/Shaders/pbr_fragment.glsl");
    m_ShadowShader = ResourceManager::loadShader("Shadow", "Resources/Shaders/shadow_vertex.glsl", "Resources/Shaders/shadow_fragment.glsl");
    m_SkyboxShader = ResourceManager::loadShader("Skybox", "Resources/Shaders/skybox_vertex.glsl", "Resources/Shaders/skybox_fragment.glsl");

    std::cout << "Renderer initialized successfully" << std::endl;
}

void Renderer::SetupLights() {
    // Setup your lights here
    std::array<PointLight, 4> pointLights;
    for (int i = 0; i < 4; ++i) {
        pointLights[i].position = glm::vec3(i * 2.0f, 3.0f, 2.0f);
        pointLights[i].color = glm::vec3(1.0f - i * 0.25f, i * 0.25f, 0.0f);
        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09f;
        pointLights[i].quadratic = 0.032f;
    }

    m_PBRShader->use();
    for (int i = 0; i < 4; ++i) {
        m_PBRShader->setPointLight("pointLights[" + std::to_string(i) + "]", pointLights[i]);
    }
}

void Renderer::Render(Scene& scene) {
    int width = m_Window->GetWidth();
    int height = m_Window->GetHeight();

    // First pass: render to shadow map
    m_ShadowShader->use();
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;
    m_ShadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    const auto& gameObjects = scene.GetGameObjects();
    for (const auto& gameObject : gameObjects) {
        if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
            glm::mat4 model = gameObject->GetTransform()->GetModelMatrix();
            m_ShadowShader->setMat4("model", model);
            renderer->Render();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Second pass: render scene as usual with PBR shader
    m_PBRShader->use();
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto activeCamera = scene.GetActiveCamera();
    if (!activeCamera) {
        std::cerr << "No active camera set in the scene!" << std::endl;
        return;
    }

    glm::mat4 view = activeCamera->GetView();
    glm::mat4 projection = activeCamera->GetProjection();
    m_PBRShader->setMat4("view", view);
    m_PBRShader->setMat4("projection", projection);
    m_PBRShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    m_PBRShader->setInt("shadowMap", 4);

    for (const auto& gameObject : gameObjects) {
        if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
            glm::mat4 model = gameObject->GetTransform()->GetModelMatrix();
            m_PBRShader->setMat4("model", model);
            renderer->Render();
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

void Renderer::Shutdown() {
    if (shadowFBO) glDeleteFramebuffers(1, &shadowFBO);
    if (shadowMap) glDeleteTextures(1, &shadowMap);
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
