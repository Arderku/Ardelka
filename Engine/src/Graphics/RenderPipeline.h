#pragma once
#include "VulkanContext.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "vulkan/vulkan.hpp"

class RenderPipeline {
public:
    RenderPipeline(VulkanContext& context, RenderPass& renderPass, SwapChain& swapChain);
    ~RenderPipeline();

    void Initialize();
    void Cleanup();
    vk::Pipeline GetPipeline() const;
    vk::PipelineLayout GetPipelineLayout() const;

private:
    VulkanContext& m_Context;
    RenderPass& m_RenderPass;
    SwapChain& m_SwapChain;
    vk::Pipeline m_Pipeline = VK_NULL_HANDLE;
    vk::PipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

    void CreateGraphicsPipeline();
    vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
    std::vector<char> ReadFile(const std::string& filename);
};
