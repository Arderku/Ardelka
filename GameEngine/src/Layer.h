#pragma once
#include <string>

class Layer
{
public:
    Layer(const std::string& name = "Layer") : m_Name(name) {}
    virtual ~Layer() {}

    // Called once when the layer is added.
    virtual void OnAttach() {}

    // Called once when the layer is removed.
    virtual void OnDetach() {}

    // Called every frame for game logic.
    virtual void OnUpdate(float deltaTime) {}

    // Called every frame for ImGui UI rendering.
    virtual void OnImGuiRender() {}

    const std::string& GetName() const { return m_Name; }

protected:
    std::string m_Name;
};
