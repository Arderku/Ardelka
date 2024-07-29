#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class Camera : public Component {
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
            : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane) {
        UpdateProjection();
    }

    void Update() override {
        UpdateView();
        UpdateFrustumPlanes();
    }

    void Render() override {
        // Rendering logic for the camera component, if needed
    }

    void UpdateProjection() {
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
    }

    void UpdateView() {
        if (m_Owner) {
            glm::vec3 position = m_Owner->GetTransform()->GetPosition();
            glm::vec3 target = position + m_Owner->GetTransform()->GetForward();
            glm::vec3 up = m_Owner->GetTransform()->GetUp();
            m_View = glm::lookAt(position, target, up);
        }
    }

    const glm::mat4& GetProjection() const { return m_Projection; }
    const glm::mat4& GetView() const { return m_View; }

    float GetFOV() const { return m_FOV; }
    void SetFOV(float fov) { m_FOV = fov; UpdateProjection(); }

    float GetAspectRatio() const { return m_AspectRatio; }
    void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; UpdateProjection(); }

    float GetNearPlane() const { return m_NearPlane; }
    void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; UpdateProjection(); }

    float GetFarPlane() const { return m_FarPlane; }
    void SetFarPlane(float farPlane) { m_FarPlane = farPlane; UpdateProjection(); }

    void UpdateFrustumPlanes() {
        glm::mat4 VP = m_Projection * m_View;
        // Left plane
        m_FrustumPlanes[0] = glm::vec4(VP[0][3] + VP[0][0], VP[1][3] + VP[1][0], VP[2][3] + VP[2][0], VP[3][3] + VP[3][0]);
        // Right plane
        m_FrustumPlanes[1] = glm::vec4(VP[0][3] - VP[0][0], VP[1][3] - VP[1][0], VP[2][3] - VP[2][0], VP[3][3] - VP[3][0]);
        // Bottom plane
        m_FrustumPlanes[2] = glm::vec4(VP[0][3] + VP[0][1], VP[1][3] + VP[1][1], VP[2][3] + VP[2][1], VP[3][3] + VP[3][1]);
        // Top plane
        m_FrustumPlanes[3] = glm::vec4(VP[0][3] - VP[0][1], VP[1][3] - VP[1][1], VP[2][3] - VP[2][1], VP[3][3] - VP[3][1]);
        // Near plane
        m_FrustumPlanes[4] = glm::vec4(VP[0][3] + VP[0][2], VP[1][3] + VP[1][2], VP[2][3] + VP[2][2], VP[3][3] + VP[3][2]);
        // Far plane
        m_FrustumPlanes[5] = glm::vec4(VP[0][3] - VP[0][2], VP[1][3] - VP[1][2], VP[2][3] - VP[2][2], VP[3][3] - VP[3][2]);

        for (int i = 0; i < 6; i++) {
            m_FrustumPlanes[i] = glm::normalize(m_FrustumPlanes[i]);
        }
    }

    const std::array<glm::vec4, 6>& GetFrustumPlanes() const { return m_FrustumPlanes; }

private:
    float m_FOV;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    std::array<glm::vec4, 6> m_FrustumPlanes;
};
