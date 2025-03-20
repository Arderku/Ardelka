#pragma once
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

class TestObject : public GameObject {
public:
    TestObject();

    void SetRotationSpeed(float speed);
    float GetRotationSpeed() const;

protected:
    void OnUpdate() override;

private:
    float m_RotationSpeed;
};
