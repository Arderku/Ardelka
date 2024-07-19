#include <iostream>
#include "GameObject.h"
#include "GLFW/glfw3.h"

GameObject::GameObject()
        : m_Transform(std::make_unique<Transform>()) {}

GameObject::~GameObject() = default;

void GameObject::AddChild(std::unique_ptr<GameObject> child) {
    m_Children.push_back(std::move(child));
}

void GameObject::AddComponent(std::unique_ptr<Component> component) {
    component->SetOwner(this);
    m_Components.push_back(std::move(component));
}

void GameObject::Update() {

    // Update logic for this GameObject
    for (const auto& component : m_Components) {
        component->Update();
    }
    for (const auto& child : m_Children) {
        child->Update();
    }
}

void GameObject::Render() {
    //log gameobject position

    for (const auto& component : m_Components) {
        component->Render();
    }
    for (const auto& child : m_Children) {
        child->Render();
    }
}

Transform* GameObject::GetTransform() const {
    return m_Transform.get();
}
