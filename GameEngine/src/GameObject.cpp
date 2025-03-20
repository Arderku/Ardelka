#include <iostream>
#include "GameObject.h"
#include "GLFW/glfw3.h"

GameObject::GameObject()
        : m_Transform(std::make_unique<Transform>()) {}

GameObject::~GameObject() = default;

void GameObject::AddChild(std::unique_ptr<GameObject> child) {
    child->m_Parent = this;  // Set the parent of the child
    child->GetTransform()->SetParent(m_Transform.get());  // Link the child's transform to the parent's transform
    m_Children.push_back(std::move(child));
}

void GameObject::AddComponent(std::unique_ptr<Component> component) {
    component->SetOwner(this);
    m_Components.push_back(std::move(component));
}

void GameObject::Update() {

    OnUpdate();

    /*float deltaTime = 0.016f; // Assume a fixed time step for simplicity, replace with actual delta time in a real scenario
    glm::vec3 rotation = GetTransform()->GetRotation();
    rotation.y += 1;
    GetTransform()->SetRotation(rotation);*/

    GetTransform()->SetDirty();

    for (const auto& component : m_Components) {
        component->Update();
    }

    // Update children
    for (const auto& child : m_Children) {
        child->Update();
    }
}

void GameObject::Render() {
    // Render logic for this GameObject
    for (const auto& component : m_Components) {
        component->Render();
    }

    // Render children
    for (const auto& child : m_Children) {
        child->Render();
    }
}

Transform* GameObject::GetTransform() const {
    return m_Transform.get();
}

std::vector<std::unique_ptr<GameObject>> &GameObject::GetChildren() {
    return m_Children;
}

std::vector<std::unique_ptr<Component>>& GameObject::GetComponents() {
    return m_Components;
}




