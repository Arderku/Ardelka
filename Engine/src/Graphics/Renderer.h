#pragma once
#include "Ardelkapch.h"
#include <Core/Window.h>
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
    gsl::not_null<Window> m_Window;
};