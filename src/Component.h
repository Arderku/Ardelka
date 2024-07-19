#pragma once

class GameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void Update() = 0;
    virtual void Render() = 0;

    void SetOwner(GameObject* owner) { m_Owner = owner; }

protected:
    GameObject* m_Owner = nullptr;
};
