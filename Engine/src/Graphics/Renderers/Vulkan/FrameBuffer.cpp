#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(VulkanContext& context, SwapChain& swapChain, RenderPass& renderPass)
        : m_Context(context), m_SwapChain(swapChain), m_RenderPass(renderPass) {}

FrameBuffer::~FrameBuffer() {
    Cleanup();
}

void FrameBuffer::Initialize() {
    CreateFrameBuffers();
}

void FrameBuffer::Cleanup() {
    for (auto framebuffer : m_FrameBuffers) {
        AR_CORE_INFO("Destroying FrameBuffer: {}", static_cast<void*>(framebuffer));
        m_Context.GetDevice().destroyFramebuffer(framebuffer);
    }
    m_FrameBuffers.clear();
}

const std::vector<vk::Framebuffer>& FrameBuffer::GetFrameBuffers() const {
    return m_FrameBuffers;
}

void FrameBuffer::CreateFrameBuffers() {
    AR_CORE_INFO("Creating FrameBuffers...");
    auto swapChainImageViews = m_SwapChain.GetImageViews();
    auto swapChainExtent = m_SwapChain.GetExtent();

    m_FrameBuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<vk::ImageView, 1> attachments = {
                swapChainImageViews[i]
        };

        vk::FramebufferCreateInfo framebufferInfo{};
        framebufferInfo.renderPass = m_RenderPass.GetRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        try {
            m_FrameBuffers[i] = m_Context.GetDevice().createFramebuffer(framebufferInfo);
            AR_CORE_INFO("FrameBuffer created for ImageView: {}", static_cast<void*>(swapChainImageViews[i]));
        } catch (vk::SystemError &err) {
            AR_CORE_ERROR("Failed to create FrameBuffer: {}", err.what());
            throw;
        }
    }
}
