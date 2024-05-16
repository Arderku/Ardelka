#include "VulkanContext.h"
#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include <set>

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

    std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VulkanContext::VulkanContext(Window& window) : m_Window(window) {}

VulkanContext::~VulkanContext() {
    if (m_Device) {
        for (auto imageView : m_SwapChainImageViews) {
            m_Device.destroyImageView(imageView);
        }
        m_SwapChainImageViews.clear();

        if (m_SwapChain) {
            m_Device.destroySwapchainKHR(m_SwapChain);
        }
        m_Device.destroy();
    }

    if (m_Instance) {
        if (m_Surface) {
            m_Instance.destroySurfaceKHR(m_Surface);
        }

        if (m_EnableValidationLayers && m_DebugMessenger) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) m_Instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(m_Instance, m_DebugMessenger, nullptr);
            }
        }

        m_Instance.destroy();
    }
}

void VulkanContext::Initialize() {
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
}

vk::Device VulkanContext::GetDevice() {
    return m_Device;
}

void VulkanContext::SetupDebugMessenger() {
    if (!m_EnableValidationLayers) return;


    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    createInfo.pfnUserCallback = DebugCallback;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) m_Instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        VkResult result = func(static_cast<VkInstance>(m_Instance), reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, reinterpret_cast<VkDebugUtilsMessengerEXT*>(&m_DebugMessenger));
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
        AR_CORE_INFO("Debug Messenger created successfully.");
    } else {
        throw std::runtime_error("Could not load vkCreateDebugUtilsMessengerEXT");
    }
}


void VulkanContext::CreateInstance() {
    uint32_t version = 0;


    // Check if validation layers are available
    std::vector<VkLayerProperties> availableLayers = GetSupportedValidationLayers();
    for (const char* layerName : m_ValidationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            AR_CORE_ERROR("Validation layer not available: {}", layerName);
            throw std::runtime_error("Required validation layers not available");
        }
    }

    vkEnumerateInstanceVersion(&version);
    version = VK_MAKE_API_VERSION(0, 1, 0, 0);

    auto appInfo = vk::ApplicationInfo(
            "Ardelka Engine",
            version,
            "Graphics Engine Dev",
            version,
            version
    );

    std::vector<const char*> extensions = GetRequiredExtensions();
    AR_CORE_INFO("GLFW required extensions: {}", extensions.size());

    auto createInfo = vk::InstanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            m_ValidationLayers.size(), m_ValidationLayers.data(),
            extensions.size(), extensions.data()
    );

    try {
        m_Instance = vk::createInstance(createInfo);
    } catch (vk::SystemError &err) {
        AR_CORE_ERROR("Failed to create Vulkan instance!");
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

std::vector<const char*> VulkanContext::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (m_EnableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

std::vector<VkLayerProperties> VulkanContext::GetSupportedValidationLayers() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    return availableLayers;
}

void VulkanContext::PickPhysicalDevice() {
    std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();
    if (devices.empty()) {
        AR_CORE_ERROR("Failed to find GPU device with Vulkan Support");
        throw std::runtime_error("Failed to find GPU device with Vulkan Support");
    }

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (!m_PhysicalDevice) {
        AR_CORE_ERROR("Failed to find a suitable GPU");
        throw std::runtime_error("Failed to find a suitable GPU");
    }
}

void VulkanContext::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    if (!indices.isComplete()) {
        throw std::runtime_error("Failed to find a complete set of queue families");
    }

    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo({}, indices.graphicsFamily.value(), 1, &queuePriority);

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo{};
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Add required device extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

    // Do not enable layers at the device level
    createInfo.enabledLayerCount = 0;

    m_Device = m_PhysicalDevice.createDevice(createInfo);
    m_GraphicsQueue = m_Device.getQueue(indices.graphicsFamily.value(), 0);
    m_PresentQueue = m_Device.getQueue(indices.presentFamily.value(), 0);

    AR_CORE_INFO_PTR("Graphics Queue: {}", m_GraphicsQueue);
    AR_CORE_INFO_PTR("Present Queue: {}", m_PresentQueue);

    // Print out device properties
    vk::PhysicalDeviceProperties properties = m_PhysicalDevice.getProperties();
    std::string deviceName(properties.deviceName);
    AR_CORE_INFO("Logical Device: {}", deviceName);
}

bool VulkanContext::IsDeviceSuitable(vk::PhysicalDevice device) {
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.geometryShader;
}

bool VulkanContext::CheckDeviceExtensionSupport(vk::PhysicalDevice device) {
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VulkanContext::QueueFamilyIndices VulkanContext::FindQueueFamilies(vk::PhysicalDevice device) {
    VulkanContext::QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vk::Result result = device.getSurfaceSupportKHR(i, m_Surface, &presentSupport);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to get surface support!");
        }

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanContext::CreateSurface() {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(m_Instance, m_Window.GetNativeWindow(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
    m_Surface = surface;
}

void VulkanContext::CreateSwapChain() {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.surface = m_Surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
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
        m_SwapChain = m_Device.createSwapchainKHR(createInfo);
        AR_CORE_INFO("Swap chain created successfully.");
    } catch (vk::SystemError& err) {
        AR_CORE_ERROR("Failed to create swap chain: {}", err.what());
        throw std::runtime_error("Failed to create swap chain!");
    }

    m_SwapChainImages = m_Device.getSwapchainImagesKHR(m_SwapChain);
    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;
}


VulkanContext::SwapChainSupportDetails VulkanContext::QuerySwapChainSupport(vk::PhysicalDevice device) {
    SwapChainSupportDetails details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(m_Surface);
    details.formats = device.getSurfaceFormatsKHR(m_Surface);
    details.presentModes = device.getSurfacePresentModesKHR(m_Surface);
    return details;
}

vk::SurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }
    return availableFormats[0]; // Return the first available format if the preferred one is not found
}

vk::PresentModeKHR VulkanContext::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo; // Return the only guaranteed present mode if the preferred one is not found
}

vk::Extent2D VulkanContext::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_Window.GetNativeWindow(), &width, &height);
        vk::Extent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

void VulkanContext::CreateImageViews() {
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
            m_SwapChainImageViews[i] = m_Device.createImageView(createInfo);
        } catch (vk::SystemError& err) {
            AR_CORE_ERROR("Failed to create image view: {}", err.what());
            throw std::runtime_error("Failed to create image view!");
        }
    }
}


void VulkanContext::CreateSyncObjects() {

}






