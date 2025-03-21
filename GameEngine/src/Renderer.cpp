#include "Renderer.h"
#include "ResourceManager.h"
#include "MeshRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(std::shared_ptr<Window> window) :
    shadowFBO(0), shadowMap(0), renderFBO(0), renderTexture(0), rboDepth(0),
    m_Window(window), m_ViewportWidth(window->GetWidth()), m_ViewportHeight(window->GetHeight()) {
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Init() {
    // Initialize GLEW and OpenGL settings

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Enable blending for transparency
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Standard alpha blending


    glClearColor(0.68f, 0.85f, 0.9f, 1.0f);

    // Setup shadow map FBO
    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    // Create and attach the depth buffer (renderbuffer)
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

void Renderer::RenderShadowPass(const glm::mat4& lightSpaceMatrix,
                                const std::vector<std::unique_ptr<GameObject>>& gameObjects) {

 glm::vec3 lightPos = glm::vec3(-1.0f, -2.0f, -1.0f);  // Set your light position here
    bool isPointLight = true;
    // Check if the light is a point light or directional light
    if (isPointLight) {
        // Set up cube map faces for point light shadows (6 faces)
        std::array<glm::mat4, 6> lightViews = {
            glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),  // Right
            glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // Left
            glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),   // Top
            glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),  // Bottom
            glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),   // Front
            glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))    // Back
        };

        glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 25.0f);  // Cube map projection for point light

        // Loop through all cube faces (6 faces)
        for (int i = 0; i < 6; ++i) {
            glm::mat4 lightSpaceMatrix = lightProjection * lightViews[i];
            m_ShadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

            // Render the scene from the current face view
            glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            for (const auto& gameObject : gameObjects) {
                if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
                    renderer->RenderShadow(*m_ShadowShader);
                }
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        // Handle directional light shadow pass (as it was before)
        m_ShadowShader->use();
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        m_ShadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Render from the directional light's view
        for (const auto& gameObject : gameObjects) {
            if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
                renderer->RenderShadow(*m_ShadowShader);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}



void Renderer::SetupLights() {
    m_PBRShader->use();

    // Point Lights setup
    float radius = 2.0f;  // Radius of the orbit (distance from the center)
float speed = 1.0f;   // Speed of the orbit (controls how fast the lights move)
float time = glfwGetTime();  // Get the current time to make the lights move

// Point Lights setup
std::array<PointLight, 4> pointLights;
for (int i = 0; i < 4; ++i) {
    // Calculate the position based on a circular orbit around the center (0, 0, 0)
    float angle = time * speed + (i * glm::pi<float>() / 2.0f);  // Offset each light by 90 degrees

    pointLights[i].position = glm::vec3(
        radius * glm::cos(angle),  // X position in the circle
        4.0f,                      // Y position (constant for vertical positioning)
        radius * glm::sin(angle)   // Z position in the circle
    );

    // Example colors (you can modify this for a more dynamic color system)
    pointLights[i].color = glm::vec3(1.0f - i * 0.25f, i * 0.25f, 0.0f);
    pointLights[i].constant = 1.0f;
    pointLights[i].linear = 0.09f;
    pointLights[i].quadratic = 0.032f;

    m_PBRShader->setPointLight("u_PointLights[" + std::to_string(i) + "]", pointLights[i]);
}

    // Directional Light setup
    glm::vec3 dirLightDirection = glm::normalize(glm::vec3(-1.0f, -2.0f, -1.0f));  // Example direction
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

    // Calculate light space transformation for shadow mapping.
    glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -1.0f);   // Example light position
    glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f)); // Example light direction
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    // Render shadow pass using the helper function.
    RenderShadowPass(lightSpaceMatrix, scene.GetGameObjects());

    // Second pass: render scene normally using PBR shader.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Bind default framebuffer.
    m_PBRShader->use();

    // Change 4: If your lights are dynamic, update them here.
    SetupLights();

    glViewport(0, 0, width, height);
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

    // Bind the shadow map texture on texture unit 4.
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    m_PBRShader->setInt("shadowMap", 4);
    m_PBRShader->setVec3("shadowColor", glm::vec3(0.2f, 0.2f, 0.3f)); // Example shadow color.

    // Change 7: Set a shadow bias uniform to help prevent shadow artifacts.
    m_PBRShader->setFloat("u_ShadowBias", 0.005f);

    const auto& gameObjects = scene.GetGameObjects();
    for (const auto& gameObject : gameObjects) {
        if (auto renderer = gameObject->GetComponent<MeshRenderer>()) {
            glm::mat4 model = gameObject->GetTransform()->GetModelMatrix();
            m_PBRShader->setMat4("model", model);
            renderer->Render();
        }
    }

    // Change 5: Optionally render a skybox to improve environmental lighting.
    if (m_SkyboxShader) {
        glDepthFunc(GL_LEQUAL);  // Ensure the skybox is rendered behind other objects.
        m_SkyboxShader->use();
        // Remove the translation from the view matrix.
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        m_SkyboxShader->setMat4("view", skyboxView);
        m_SkyboxShader->setMat4("projection", projection);
        // Call your skybox rendering code here, for example:
        // skybox->Render();
        glDepthFunc(GL_LESS);  // Restore default depth function.
    }

    // Change 6: Optionally, add a post-processing pass here for tone mapping and gamma correction.

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

    // Calculate light space transformation for shadow mapping.
    glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -1.0f);
    glm::vec3 lightDir = glm::normalize(glm::vec3(-2.0f, -4.0f, -1.0f));
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    // Render shadow pass.
    RenderShadowPass(lightSpaceMatrix, scene.GetGameObjects());

    // Render scene to the off-screen framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
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
    m_PBRShader->setFloat("u_ShadowBias", 0.005f);

    const auto& gameObjects = scene.GetGameObjects();
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

    // Update the render texture to match the new viewport dimensions.
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Change 3: Update the renderbuffer storage using the member variable rboDepth.
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: "
                  << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
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
