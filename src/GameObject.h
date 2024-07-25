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
    virtual ~GameObject();

    void SetName(std::string name) { m_Name = name; }
    std::string GetName() const { return m_Name; }

    void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
    bool IsStatic() const { return m_IsStatic; }

    void AddChild(std::unique_ptr<GameObject> child);
    void AddComponent(std::unique_ptr<Component> component);
    void Update();
    void Render();
    Transform* GetTransform() const;

    GameObject* GetParent() const { return m_Parent; }
    std::vector<std::unique_ptr<GameObject>>& GetChildren();


    std::vector<std::unique_ptr<Component>>& GetComponents();

    template<typename T>
    T* GetComponent() {
        for (auto& component : m_Components) {
            T* t = dynamic_cast<T*>(component.get());
            if (t) {
                return t;
            }
        }
        return nullptr;
    }


    virtual void OnUpdate() {
    }

private:

    std::unique_ptr<Transform> m_Transform;
    std::vector<std::unique_ptr<Component>> m_Components;
    std::vector<std::unique_ptr<GameObject>> m_Children;

    GameObject* m_Parent = nullptr;  // Add parent pointer
    std::string m_Name = "GameObject"; // Name of the GameObject
    bool m_IsStatic = false;
};
