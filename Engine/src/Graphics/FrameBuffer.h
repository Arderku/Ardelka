#pragma once
#include "VulkanContext.h"
#include "Ardelkapch.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "vulkan/vulkan.hpp"


class FrameBuffer {
public:
    FrameBuffer(VulkanContext& context, SwapChain& swapChain, RenderPass& renderPass);
    ~FrameBuffer();

    void Initialize();
    void Cleanup();
    const std::vector<vk::Framebuffer>& GetFrameBuffers() const;

private:
    VulkanContext& m_Context;
    SwapChain& m_SwapChain;
    RenderPass& m_RenderPass;
    std::vector<vk::Framebuffer> m_FrameBuffers;

    void CreateFrameBuffers();
};
