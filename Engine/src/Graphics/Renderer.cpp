#include <future>
#include "Renderer.h"

Renderer::Renderer(Window& window)
        : m_Window(window),
          m_VulkanContext(window),
          m_SwapChain(m_VulkanContext)
          //m_FrameManager(m_VulkanContext)
          {}


Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Initialize() {
    m_VulkanContext.Initialize();
    m_SwapChain.Initialize();
    //m_FrameManager.Initialize();
}

void Renderer::Render() {

}

void Renderer::Shutdown() {
    AR_CORE_INFO("Shutting down Renderer...");
    std::future<void> future = std::async(std::launch::async, [&]() {
        m_SwapChain.Cleanup();
    });

    // Wait for the SwapChain cleanup to finish
    future.wait();
    m_VulkanContext.Cleanup();
}
