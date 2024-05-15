#pragma once
#include "Ardelkapch.h"
#include <Core/Window.h>
#include "VulkanContext.h"


class Renderer final{
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void Render();
    void Shutdown();
private:
    VulkanContext m_VulkanContext;
};