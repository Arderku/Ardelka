#include "SwapChain.h"


SwapChain::SwapChain(VulkanContext& context) : m_Context(context) {
    AR_CORE_INFO("SwapChain initialized with context.");
}

SwapChain::~SwapChain() {
    Cleanup();
}

void SwapChain::Initialize() {
    AR_CORE_INFO("Initializing SwapChain...");
    CreateSwapChain();
    CreateImageViews();
}

void SwapChain::Cleanup() {
    AR_CORE_INFO("Cleaning up SwapChain...");
    for (auto imageView : m_SwapChainImageViews) {
        AR_CORE_INFO("Destroying ImageView: {}", static_cast<void*>(imageView));
        m_Context.GetDevice().destroyImageView(imageView);
    }
    m_SwapChainImageViews.clear();

    if (m_SwapChain) {
        AR_CORE_INFO("Destroying SwapChain: {}", static_cast<void*>(m_SwapChain));
        m_Context.GetDevice().destroySwapchainKHR(m_SwapChain);
        m_SwapChain = VK_NULL_HANDLE;
    }

    AR_CORE_INFO("SwapChain cleaned up.");
}

vk::SwapchainKHR SwapChain::GetSwapChain() const {
    return m_SwapChain;
}

vk::Format SwapChain::GetSwapChainImageFormat() const {
    return m_SwapChainImageFormat;
}

void SwapChain::CreateSwapChain() {
    AR_CORE_INFO("Creating SwapChain...");
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_Context.GetPhysicalDevice(), m_Context.GetSurface());

    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.surface = m_Context.GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    VulkanContext::QueueFamilyIndices indices = m_Context.FindQueueFamilies(m_Context.GetPhysicalDevice());
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    try {
        m_SwapChain = m_Context.GetDevice().createSwapchainKHR(createInfo);
        AR_CORE_INFO("SwapChain created successfully.");
    } catch (vk::SystemError &err) {
        AR_CORE_ERROR("Failed to create SwapChain: {}", err.what());
        throw;
    }

    m_SwapChainImages = m_Context.GetDevice().getSwapchainImagesKHR(m_SwapChain);
    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;

    AR_CORE_INFO("SwapChain Images: {}", m_SwapChainImages.size());
}

void SwapChain::CreateImageViews() {
    AR_CORE_INFO("Creating ImageViews...");
    m_SwapChainImageViews.resize(m_SwapChainImages.size());

    for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo{};
        createInfo.image = m_SwapChainImages[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_SwapChainImageFormat;
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        try {
            m_SwapChainImageViews[i] = m_Context.GetDevice().createImageView(createInfo);
            AR_CORE_INFO("ImageView created for Image: {}", i);
        } catch (vk::SystemError &err) {
            AR_CORE_ERROR("Failed to create ImageView for Image {}: {}", i, err.what());
            throw;
        }
    }
}

vk::SurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }
    return availableFormats[0]; // Return the first available format if the preferred one is not found
}

vk::PresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo; // Return the only guaranteed present mode if the preferred one is not found
}

vk::Extent2D SwapChain::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_Context.GetWindow().GetNativeWindow(), &width, &height);
        vk::Extent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

SwapChain::SwapChainSupportDetails SwapChain::QuerySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface) {
    AR_CORE_INFO("Querying SwapChain support...");
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    details.formats = device.getSurfaceFormatsKHR(surface);
    details.presentModes = device.getSurfacePresentModesKHR(surface);
    return details;
}


