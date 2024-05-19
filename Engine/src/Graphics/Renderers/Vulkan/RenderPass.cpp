#include "RenderPass.h"

RenderPass::RenderPass(VulkanContext& context) : m_Context(context) {}

RenderPass::~RenderPass() {
    Cleanup();
}

void RenderPass::Initialize(vk::Format swapChainImageFormat) {
    CreateRenderPass(swapChainImageFormat);
}

void RenderPass::Cleanup() {
    if (m_RenderPass) {
        AR_CORE_INFO("Destroying RenderPass...");
        m_Context.GetDevice().destroyRenderPass(m_RenderPass);
        m_RenderPass = VK_NULL_HANDLE;
    }
}

vk::RenderPass RenderPass::GetRenderPass() const {
    return m_RenderPass;
}

void RenderPass::CreateRenderPass(vk::Format swapChainImageFormat) {
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = {};
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo renderPassInfo{};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    try {
        m_RenderPass = m_Context.GetDevice().createRenderPass(renderPassInfo);
        AR_CORE_INFO("RenderPass created successfully.");
    } catch (vk::SystemError &err) {
        AR_CORE_ERROR("Failed to create RenderPass: {}", err.what());
        throw;
    }
}
