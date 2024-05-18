#pragma once
#include "Ardelkapch.h"
#include "VulkanContext.h"
#include "vulkan/vulkan.hpp"

class RenderPass {
public:
    RenderPass(VulkanContext& context);
    ~RenderPass();

    void Initialize(vk::Format swapChainImageFormat);
    void Cleanup();
    vk::RenderPass GetRenderPass() const;

private:
    VulkanContext& m_Context;
    vk::RenderPass m_RenderPass = VK_NULL_HANDLE;

    void CreateRenderPass(vk::Format swapChainImageFormat);
};
