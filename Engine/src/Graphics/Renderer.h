#pragma once
#include "Ardelkapch.h"
#include <Core/Window.h>
#include "VulkanContext.h"


class Renderer final{
public:
    Renderer(Window& window);
    ~Renderer();

    void Initialize();
    void Render();
    void Shutdown();
private:
    Window& m_Window;
    VulkanContext m_VulkanContext;
};

