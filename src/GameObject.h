#pragma once
#include <vector>
#include <memory>
#include <string> // Include this header for std::string
#include "Transform.h"
#include "Component.h"
#include <glm/glm.hpp>

class GameObject {
public:
    GameObject();
    ~GameObject();

    void SetName(std::string name) { m_Name = name; }
    std::string GetName() const { return m_Name; }

    void AddChild(std::unique_ptr<GameObject> child);
    void AddComponent(std::unique_ptr<Component> component);
    void Update();
    void Render();
    Transform* GetTransform() const;

    GameObject* GetParent() const { return m_Parent; }
    std::vector<std::unique_ptr<GameObject>>& GetChildren();


private:
    std::unique_ptr<Transform> m_Transform;
    std::vector<std::unique_ptr<Component>> m_Components;
    std::vector<std::unique_ptr<GameObject>> m_Children;

    GameObject* m_Parent = nullptr;  // Add parent pointer
    std::string m_Name = "GameObject"; // Name of the GameObject
};
