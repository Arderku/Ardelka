#include <future>
#include "Renderer.h"

Renderer::Renderer(Window &window)
        : m_Window(window),
          m_VulkanContext(window),
          m_SwapChain(m_VulkanContext),
          m_RenderPass(m_VulkanContext),
          m_FrameBuffer(m_VulkanContext, m_SwapChain, m_RenderPass),
          m_RenderPipeline(m_VulkanContext, m_RenderPass, m_SwapChain),
          m_CommandBufferManager(m_VulkanContext, m_FrameBuffer, m_RenderPipeline, m_RenderPass, m_SwapChain)
          {}


Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Initialize() {
    m_VulkanContext.Initialize();
    m_SwapChain.Initialize();
    m_RenderPass.Initialize(m_SwapChain.GetImageFormat());
    m_FrameBuffer.Initialize();
    m_RenderPipeline.Initialize();
    m_CommandBufferManager.Initialize();
}

void Renderer::Render() {
    m_CommandBufferManager.DrawFrame();
}

void Renderer::Shutdown() {
    AR_CORE_INFO("Shutting down Renderer...");
    m_CommandBufferManager.Cleanup();
    m_RenderPipeline.Cleanup();
    m_FrameBuffer.Cleanup();
    m_RenderPass.Cleanup();

    std::future<void> future = std::async(std::launch::async, [&]() {
        m_SwapChain.Cleanup();
    });

    // Wait for the SwapChain cleanup to finish
    future.wait();
    m_VulkanContext.Cleanup();
}
