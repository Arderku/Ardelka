#include "VulkanContext.h"
#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

    AR_CORE_ERROR("Validation Layer: {}", pCallbackData->pMessage);

    return VK_FALSE;
}

VulkanContext::VulkanContext(Window& window) : m_Window(window) {}

VulkanContext::~VulkanContext() {
    if (m_Device) {
        m_Device.destroySwapchainKHR(m_SwapChain);
        m_Device.destroy();
    }
    if (m_Instance) {
        if (m_Surface) {
            m_Instance.destroySurfaceKHR(m_Surface);
        }
        m_Instance.destroy();
    }
}

void VulkanContext::Initialize() {
    CreateInstance();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
}

vk::Device VulkanContext::GetDevice() {
    return m_Device;
}

void VulkanContext::CreateInstance() {
    uint32_t version = 0;

    // Validation layers
    std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    // Check if validation layers are available
    std::vector<VkLayerProperties> availableLayers = GetSupportedValidationLayers();
    for (const char* layerName : validationLayers) {
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
            validationLayers.size(), validationLayers.data(),
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
    }

    for (const auto& device : devices) {
        vk::PhysicalDeviceProperties properties = device.getProperties();
        std::string deviceName(properties.deviceName);
        AR_CORE_INFO("Device: {}", deviceName);
    }

    m_PhysicalDevice = devices[2];
}

void VulkanContext::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    if (!indices.isComplete()) {
        throw std::runtime_error("Failed to find a complete set of queue families");
    }

    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo({}, indices.graphicsFamily.value(), 1, &queuePriority);
    vk::DeviceCreateInfo createInfo({}, 1, &queueCreateInfo);

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

bool VulkanContext::IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers) {
    std::vector<vk::ExtensionProperties> availableExtensions = m_PhysicalDevice.enumerateDeviceExtensionProperties();

    for (const char* extension : extensions) {
        bool extensionFound = false;
        for (const auto& availableExtension : availableExtensions) {
            if (strcmp(extension, availableExtension.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }
        if (!extensionFound) {
            return false;
        }
    }

    vk::PhysicalDeviceFeatures supportedFeatures = m_PhysicalDevice.getFeatures();
    if (!supportedFeatures.geometryShader) {
        return false;
    }

    return true;
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
    if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
        AR_CORE_ERROR("Swap chain support is inadequate!");
        throw std::runtime_error("Swap chain support is inadequate!");
    }

    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    AR_CORE_INFO("Swap surface format chosen: Format {}, Color Space {}", vk::to_string(surfaceFormat.format), vk::to_string(surfaceFormat.colorSpace));
    vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    AR_CORE_INFO("Swap present mode chosen: {}", vk::to_string(presentMode));
    vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
    AR_CORE_INFO("Swap extent chosen: Width {}, Height {}", extent.width, extent.height);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    AR_CORE_INFO("Image count set: {}", imageCount);

    vk::SwapchainCreateInfoKHR createInfo(
            vk::SwapchainCreateFlagsKHR(),
            m_Surface,
            imageCount,
            surfaceFormat.format,
            surfaceFormat.colorSpace,
            extent,
            1,
            vk::ImageUsageFlagBits::eColorAttachment
    );

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
    createInfo.oldSwapchain = nullptr;

    AR_CORE_INFO("Creating swap chain...");
    try {
        m_SwapChain = m_Device.createSwapchainKHR(createInfo);
        AR_CORE_INFO("Swap chain created successfully.");
    } catch (vk::SystemError& err) {
        AR_CORE_ERROR("Failed to create swap chain: {}", err.what());
        throw std::runtime_error("Failed to create swap chain!");
    }

    // Retrieve the swap chain images
    std::vector<vk::Image> swapChainImages = m_Device.getSwapchainImagesKHR(m_SwapChain);
    m_SwapChainImages.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        m_SwapChainImages[i] = swapChainImages[i];
    }
    AR_CORE_INFO("Retrieved {} swap chain images.", swapChainImages.size());

    // Store the format and extent for later use
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



