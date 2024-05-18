#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "Ardelkapch.h"
#include "VulkanContext.h"
#include "FrameBuffer.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include "SwapChain.h"

class CommandBufferManager {
public:
    CommandBufferManager(VulkanContext& context, FrameBuffer& frameBuffer, RenderPipeline& renderPipeline, RenderPass& renderPass, SwapChain& swapChain);
    ~CommandBufferManager();

    void Initialize();
    void Cleanup();
    void DrawFrame();

private:
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSyncObjects();

    VulkanContext& m_Context;
    FrameBuffer& m_FrameBuffer;
    RenderPipeline& m_RenderPipeline;
    RenderPass& m_RenderPass;
    SwapChain& m_SwapChain;

    vk::CommandPool m_CommandPool;
    std::vector<vk::CommandBuffer> m_CommandBuffers;


    vk::Semaphore m_ImageAvailableSemaphore;
    vk::Semaphore m_RenderFinishedSemaphore;
    vk::Fence m_InFlightFence;
};
