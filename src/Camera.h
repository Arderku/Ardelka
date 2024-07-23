#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Component {
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
            : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane) {
        UpdateProjection();
    }

    void Update() override {
        // Update the view matrix using the GameObject's transform
        UpdateView();
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

private:
    float m_FOV;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
};
