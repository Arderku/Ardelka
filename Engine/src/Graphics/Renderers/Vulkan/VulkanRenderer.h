#pragma once
#include "Ardelkapch.h"
#include "Core/Window.h"
#include "VulkanContext.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "RenderPipeline.h"
#include "CommandBufferManager.h"


class VulkanRenderer {
public:
    VulkanRenderer(Window& window);
    ~VulkanRenderer();

    void Initialize();
    void Render();
    void Shutdown();

private:
    Window& m_Window;
    VulkanContext m_VulkanContext;
    SwapChain m_SwapChain;
    RenderPass m_RenderPass;
    FrameBuffer m_FrameBuffer;
    RenderPipeline m_RenderPipeline;
    CommandBufferManager m_CommandBufferManager;
};
