#pragma once
#include "Ardelkapch.h"
#include "Core/Window.h"
#include "VulkanContext.h"
#include "SwapChain.h"
#include "RenderPass.h"


class Renderer {
public:
    Renderer(Window& window);
    ~Renderer();

    void Initialize();
    void Render();
    void Shutdown();

private:
    Window& m_Window;
    VulkanContext m_VulkanContext;
    SwapChain m_SwapChain;
    RenderPass m_RenderPass;

};
