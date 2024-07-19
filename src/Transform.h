#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component {
public:
    Transform()
            : position(0.0f), rotation(0.0f), scale(1.0f), parent(nullptr) {}

    void Update() override {}
    void Render() override {}

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        if (parent) {
            model = parent->GetModelMatrix() * model;
        }

        return model;
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Transform* parent;
};
