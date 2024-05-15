#include "Renderer.h"

Renderer::Renderer() {
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Initialize() {
    m_VulkanContext.Initialize();
}

void Renderer::Render() {
    vk::Device device = m_VulkanContext.GetDevice();
}

void Renderer::Shutdown() {
}
