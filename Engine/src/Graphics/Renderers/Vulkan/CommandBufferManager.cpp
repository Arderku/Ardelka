#include "CommandBufferManager.h"

CommandBufferManager::CommandBufferManager(VulkanContext& context, FrameBuffer& frameBuffer, RenderPipeline& renderPipeline, RenderPass& renderPass, SwapChain& swapChain)
        : m_Context(context), m_FrameBuffer(frameBuffer), m_RenderPipeline(renderPipeline), m_RenderPass(renderPass), m_SwapChain(swapChain) {}

CommandBufferManager::~CommandBufferManager() {
    Cleanup();
}

void CommandBufferManager::Initialize() {
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();
}

void CommandBufferManager::Cleanup() {
    m_Context.GetDevice().waitIdle();

    if (m_CommandPool) {
        AR_CORE_INFO("Destroying Command Pool...");
        m_Context.GetDevice().destroyCommandPool(m_CommandPool);
    }
    if (m_ImageAvailableSemaphore) {
        AR_CORE_INFO("Destroying Image Available Semaphore...");
        m_Context.GetDevice().destroySemaphore(m_ImageAvailableSemaphore);
    }
    if (m_RenderFinishedSemaphore) {
        AR_CORE_INFO("Destroying Render Finished Semaphore...");
        m_Context.GetDevice().destroySemaphore(m_RenderFinishedSemaphore);
    }
    if (m_InFlightFence) {
        AR_CORE_INFO("Destroying In Flight Fence...");
        m_Context.GetDevice().destroyFence(m_InFlightFence);
    }
}

void CommandBufferManager::CreateCommandPool() {
    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = m_Context.GetGraphicsQueueFamilyIndex();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try {
        m_CommandPool = m_Context.GetDevice().createCommandPool(poolInfo);
        AR_CORE_INFO("Command Pool created successfully.");
    } catch (vk::SystemError& err) {
        AR_CORE_ERROR("Failed to create Command Pool: {}", err.what());
        throw;
    }
}

void CommandBufferManager::CreateCommandBuffers() {
    m_CommandBuffers.resize(m_FrameBuffer.GetFrameBuffers().size());

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = m_CommandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

    try {
        m_CommandBuffers = m_Context.GetDevice().allocateCommandBuffers(allocInfo);
        AR_CORE_INFO("Command Buffers allocated successfully.");
    } catch (vk::SystemError& err) {
        AR_CORE_ERROR("Failed to allocate Command Buffers: {}", err.what());
        throw;
    }

    for (size_t i = 0; i < m_CommandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo{};

        m_CommandBuffers[i].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassInfo{};
        renderPassInfo.renderPass = m_RenderPass.GetRenderPass();
        renderPassInfo.framebuffer = m_FrameBuffer.GetFrameBuffers()[i];
        renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
        renderPassInfo.renderArea.extent = m_SwapChain.GetExtent();

        vk::ClearValue clearColor = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.055f, 0.439f, 0.443f, 1.0f}));
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        m_CommandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        m_CommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_RenderPipeline.GetPipeline());

        // No draw commands, just clear the color
        //m_CommandBuffers[i].draw(3, 1, 0, 0);

        m_CommandBuffers[i].endRenderPass();

        m_CommandBuffers[i].end();
    }
}

void CommandBufferManager::CreateSyncObjects() {
    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    try {
        m_ImageAvailableSemaphore = m_Context.GetDevice().createSemaphore(semaphoreInfo);
        m_RenderFinishedSemaphore = m_Context.GetDevice().createSemaphore(semaphoreInfo);
        m_InFlightFence = m_Context.GetDevice().createFence(fenceInfo);
        AR_CORE_INFO("Synchronization objects created successfully.");
    } catch (vk::SystemError& err) {
        AR_CORE_ERROR("Failed to create synchronization objects: {}", err.what());
        throw;
    }
}

void CommandBufferManager::DrawFrame() {
    // Wait for the previous frame to finish
    vk::Result result = m_Context.GetDevice().waitForFences(1, &m_InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to wait for fences!");
    }

    result = m_Context.GetDevice().resetFences(1, &m_InFlightFence);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to reset fences!");
    }

    uint32_t imageIndex;
    result = m_Context.GetDevice().acquireNextImageKHR(m_SwapChain.GetSwapChain(), std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphore, nullptr, &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        // Handle swap chain recreation if needed
        return;
    } else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    vk::SubmitInfo submitInfo{};

    vk::Semaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];

    vk::Semaphore signalSemaphores[] = { m_RenderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = m_Context.GetGraphicsQueue().submit(1, &submitInfo, m_InFlightFence);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { m_SwapChain.GetSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = m_Context.GetPresentQueue().presentKHR(&presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
        // Handle swap chain recreation if needed
    } else if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}

