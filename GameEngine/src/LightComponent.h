#pragma once
#include "Component.h"
#include <glm/glm.hpp>

enum class LightType {
    Point,
    Directional,
    Spot
};

class LightComponent : public Component {
public:
    LightComponent() : m_LightType(LightType::Point), m_Color(1.0f) {}
    virtual ~LightComponent() = default;

    void SetLightType(LightType type) { m_LightType = type; }
    LightType GetLightType() const { return m_LightType; }

    void SetColor(const glm::vec3& color) { m_Color = color; }
    glm::vec3 GetColor() const { return m_Color; }

    virtual void Update() override = 0;
    virtual void Render() override = 0;

protected:
    LightType m_LightType;
    glm::vec3 m_Color;
};

class PointLightComponent : public LightComponent {
public:
    PointLightComponent() {
        m_LightType = LightType::Point;
        m_Constant = 1.0f;
        m_Linear = 0.09f;
        m_Quadratic = 0.032f;
    }

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }

    void SetConstant(float constant) { m_Constant = constant; }
    float GetConstant() const { return m_Constant; }

    void SetLinear(float linear) { m_Linear = linear; }
    float GetLinear() const { return m_Linear; }

    void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }
    float GetQuadratic() const { return m_Quadratic; }

    void Update() override {}
    void Render() override {}

private:
    glm::vec3 m_Position;
    float m_Constant;
    float m_Linear;
    float m_Quadratic;
};

class DirectionalLightComponent : public LightComponent {
public:
    DirectionalLightComponent() {
        m_LightType = LightType::Directional;
        m_Direction = glm::vec3(-2.0f, -4.0f, -1.0f);
    }

    void SetDirection(const glm::vec3& direction) { m_Direction = direction; }
    glm::vec3 GetDirection() const { return m_Direction; }

    void Update() override {}
    void Render() override {}

private:
    glm::vec3 m_Direction;
};

class SpotLightComponent : public LightComponent {
public:
    SpotLightComponent() {
        m_LightType = LightType::Spot;
        m_Cutoff = 12.5f;
        m_OuterCutoff = 15.0f;
    }

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }

    void SetDirection(const glm::vec3& direction) { m_Direction = direction; }
    glm::vec3 GetDirection() const { return m_Direction; }

    void SetCutoff(float cutoff) { m_Cutoff = cutoff; }
    float GetCutoff() const { return m_Cutoff; }

    void SetOuterCutoff(float outerCutoff) { m_OuterCutoff = outerCutoff; }
    float GetOuterCutoff() const { return m_OuterCutoff; }

    void Update() override {}
    void Render() override {}

private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    float m_Cutoff;
    float m_OuterCutoff;
};
