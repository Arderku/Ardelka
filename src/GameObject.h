#pragma once
#include <vector>
#include <memory>
#include "Transform.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    void AddChild(std::unique_ptr<GameObject> child);
    void AddComponent(std::unique_ptr<Component> component);
    void Update();
    void Render();
    Transform* GetTransform() const;

private:
    std::unique_ptr<Transform> m_Transform;
    std::vector<std::unique_ptr<Component>> m_Components;
    std::vector<std::unique_ptr<GameObject>> m_Children;
};

