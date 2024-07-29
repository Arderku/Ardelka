#include "TestObject.h"

TestObject::TestObject() : m_RotationSpeed(1.0f) {}

void TestObject::SetRotationSpeed(float speed) {
    m_RotationSpeed = speed;
}

float TestObject::GetRotationSpeed() const {
    return m_RotationSpeed;
}

void TestObject::OnUpdate() {
    float deltaTime = 0.016f; // Assume a fixed time step for simplicity, replace with actual delta time in a real scenario
    glm::vec3 rotation = GetTransform()->GetRotation();
    rotation.y += m_RotationSpeed;
    GetTransform()->SetRotation(rotation);

    GetTransform()->SetDirty();
}
