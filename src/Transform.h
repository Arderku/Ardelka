#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component {
public:
    Transform()
            : position(0.0f), rotation(0.0f), scale(1.0f), parent(nullptr), isDirty(true) {}

    void Update() override {}
    void Render() override {}

    glm::mat4 GetModelMatrix() const {
        if (isDirty) {
            localMatrix = glm::translate(glm::mat4(1.0f), position);
            localMatrix = glm::rotate(localMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            localMatrix = glm::rotate(localMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            localMatrix = glm::rotate(localMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            localMatrix = glm::scale(localMatrix, scale);
            isDirty = false;
        }
        return localMatrix;
    }

    glm::mat4 GetGlobalModelMatrix() const {
        if (globalIsDirty) {
            if (parent) {
                globalMatrix = parent->GetGlobalModelMatrix() * GetModelMatrix();
            } else {
                globalMatrix = GetModelMatrix();
            }
            globalIsDirty = false;
        }
        return globalMatrix;
    }

    void SetDirty() {
        isDirty = true;
        globalIsDirty = true;
    }

    void SetParent(Transform* parentTransform) {
        parent = parentTransform;
        SetDirty();
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec3 GetPosition() {
        return position;
    }

    void SetPosition(glm::vec3 vec1) {
        position = vec1;
        SetDirty();
    }

    glm::vec3 GetRotation() {
        return rotation;
    }

    void SetRotation(glm::vec3 vec1) {
        rotation = vec1;
        SetDirty();
    }

    glm::vec3 GetScale() {
        return scale;
    }

    void SetScale(glm::vec3 vec1) {
        scale = vec1;
        SetDirty();
    }

private:
    mutable glm::mat4 localMatrix;
    mutable glm::mat4 globalMatrix;
    mutable bool isDirty;
    mutable bool globalIsDirty;

    Transform* parent;
};
