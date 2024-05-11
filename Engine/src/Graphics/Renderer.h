#pragma once
#include "VulkanContext.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void Render();
    void Shutdown();
private:
    VulkanContext m_VulkanContext;
};