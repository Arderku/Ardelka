#include "Renderer.h"
#include "ResourceManager.h"
#include "MeshRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(std::shared_ptr<Window> window) : shadowFBO(0), shadowMap(0), renderFBO(0), renderTexture(0), m_Window(window) {
}

Renderer::~Renderer() {
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

    // Setup render FBO and texture
    glGenFramebuffers(1, &renderFBO);
    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    // Create and attach the depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Window->GetWidth(), m_Window->GetHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Load shaders
    m_PBRShader = ResourceManager::loadShader("PBR", "Resources/Shaders/pbr_vertex.glsl", "Resources/Shaders/pbr_fragment.glsl");
    m_ShadowShader = ResourceManager::loadShader("Shadow", "Resources/Shaders/shadow_vertex.glsl", "Resources/Shaders/shadow_fragment.glsl");
    m_SkyboxShader = ResourceManager::loadShader("Skybox", "Resources/Shaders/skybox_vertex.glsl", "Resources/Shaders/skybox_fragment.glsl");

    SetupLights();
    std::cout << "Renderer initialized successfully" << std::endl;
}

void Renderer::SetupLights() {
    m_PBRShader->use();

    // Point Lights setup
    std::array<PointLight, 4> pointLights;
    for (int i = 0; i < 4; ++i) {
        pointLights[i].position = glm::vec3(i * 2.0f, 3.0f, 2.0f);  // Example positions
        pointLights[i].color = glm::vec3(1.0f - i * 0.25f, i * 0.25f, 0.0f);  // Example colors
        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09f;
        pointLights[i].quadratic = 0.032f;
        m_PBRShader->setPointLight("pointLights[" + std::to_string(i) + "]", pointLights[i]);
    }

    // Directional Light setup
    glm::vec3 dirLightDirection = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));  // Example direction
    glm::vec3 dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // White color

    DirectionalLight dirLight;
    dirLight.direction = dirLightDirection;
    dirLight.color = dirLightColor;

    m_PBRShader->setDirectionalLight("u_DirectionalLight", dirLight);

    // Ambient Light setup
    glm::vec3 ambientColor = glm::vec3(0.5f, 0.5f, 0.5f);  // Neutral grey
    float ambientIntensity = 0.8f;  // Bright ambient light
    m_PBRShader->setVec3("u_AmbientLight.color", ambientColor);
    m_PBRShader->setFloat("u_AmbientLight.intensity", ambientIntensity);
}

void Renderer::Render(Scene& scene) {
    int width = m_Window->GetWidth();
    int height = m_Window->GetHeight();

    // First pass: render to shadow map
    m_ShadowShader->use();
    glViewport(0, 0, 2048, 2048);  // Updated to match new resolution
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -1.0f);  // Example light position
    glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));  // Example light direction

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Bind the default framebuffer
    m_PBRShader->use();
    SetupLights();
    glViewport(0, 0, width, height); // Use window dimensions
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto activeCamera = scene.GetActiveCamera();
    if (!activeCamera) {
        std::cerr << "No active camera set in the scene!" << std::endl;
        return;
    }
    activeCamera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    glm::mat4 view = activeCamera->GetView();
    glm::mat4 projection = activeCamera->GetProjection();
    m_PBRShader->setMat4("view", view);
    m_PBRShader->setMat4("projection", projection);
    m_PBRShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    m_PBRShader->setInt("shadowMap", 4);
    m_PBRShader->setVec3("shadowColor", glm::vec3(0.2f, 0.2f, 0.3f)); // Example shadow color

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

void Renderer::RenderToViewport(Scene& scene, int viewportWidth, int viewportHeight) {
    if (viewportWidth != m_ViewportWidth || viewportHeight != m_ViewportHeight) {
        UpdateViewportSize(viewportWidth, viewportHeight);
    }

    // First pass: render to shadow map
    m_ShadowShader->use();
    glViewport(0, 0, 2048, 2048);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -1.0f);
    glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
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

    // Second pass: render scene to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO); // Bind the render FBO
    m_PBRShader->use();
    SetupLights();
    glViewport(0, 0, viewportWidth, viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto activeCamera = scene.GetActiveCamera();
    if (!activeCamera) {
        std::cerr << "No active camera set in the scene!" << std::endl;
        return;
    }
    activeCamera->SetAspectRatio(static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight));
    glm::mat4 view = activeCamera->GetView();
    glm::mat4 projection = activeCamera->GetProjection();
    m_PBRShader->setMat4("view", view);
    m_PBRShader->setMat4("projection", projection);
    m_PBRShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    m_PBRShader->setInt("shadowMap", 4);
    m_PBRShader->setVec3("shadowColor", glm::vec3(0.2f, 0.2f, 0.3f));

    for (const auto& gameObject : gameObjects) {
        if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
            glm::mat4 model = gameObject->GetTransform()->GetModelMatrix();
            m_PBRShader->setMat4("model", model);
            renderer->Render();
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after rendering: " << error << std::endl;
    }
#endif
}

void Renderer::UpdateViewportSize(int width, int height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    // Update the framebuffer size to match the viewport size
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Shutdown() {
    if (shadowFBO) glDeleteFramebuffers(1, &shadowFBO);
    if (shadowMap) glDeleteTextures(1, &shadowMap);
    if (renderFBO) glDeleteFramebuffers(1, &renderFBO);
    if (renderTexture) glDeleteTextures(1, &renderTexture);
    if (rboDepth) glDeleteRenderbuffers(1, &rboDepth);
}

GLuint Renderer::GetRenderTexture() const {
    return renderTexture;
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
