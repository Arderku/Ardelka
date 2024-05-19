#include "VulkanContext.h"
#include "Ardelkapch.h"
#include "SwapChain.h"  // Include SwapChain to use its static method
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

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
    Cleanup();
}

void VulkanContext::Initialize() {
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void VulkanContext::Cleanup() {
    if (m_Device) {
        AR_CORE_INFO("Destroying Vulkan device...");
        m_Device.destroy();
        m_Device = VK_NULL_HANDLE;
    }

    if (m_Instance) {
        if (m_Surface) {
            AR_CORE_INFO("Destroying Vulkan surface...");
            m_Instance.destroySurfaceKHR(m_Surface);
            m_Surface = VK_NULL_HANDLE;
        }

        if (m_EnableValidationLayers && m_DebugMessenger) {
            AR_CORE_INFO("Destroying Vulkan debug messenger...");
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) m_Instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(m_Instance, m_DebugMessenger, nullptr);
            }
            m_DebugMessenger = VK_NULL_HANDLE;
        }

        AR_CORE_INFO("Destroying Vulkan instance...");
        m_Instance.destroy();
        m_Instance = VK_NULL_HANDLE;
    }
}

Window &VulkanContext::GetWindow() const {
    return m_Window;
}

vk::Device VulkanContext::GetDevice() const {
    return m_Device;
}

vk::PhysicalDevice VulkanContext::GetPhysicalDevice() const {
    return m_PhysicalDevice;
}

vk::Instance VulkanContext::GetInstance() const {
    return m_Instance;
}

vk::SurfaceKHR VulkanContext::GetSurface() const {
    return m_Surface;
}

uint32_t VulkanContext::GetGraphicsQueueFamilyIndex() const {
    return FindQueueFamilies(m_PhysicalDevice).graphicsFamily.value();
}

vk::Queue VulkanContext::GetGraphicsQueue() const {
    return m_Device.getQueue(FindQueueFamilies(m_PhysicalDevice).graphicsFamily.value(), 0);
}

vk::Queue VulkanContext::GetPresentQueue() const {
    return m_Device.getQueue(FindQueueFamilies(m_PhysicalDevice).presentFamily.value(), 0);
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
    } else {
        throw std::runtime_error("Could not load vkCreateDebugUtilsMessengerEXT");
    }
}

void VulkanContext::CreateInstance() {
    uint32_t version = 0;

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

    auto createInfo = vk::InstanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            m_ValidationLayers.size(), m_ValidationLayers.data(),
            extensions.size(), extensions.data()
    );

    try {
        m_Instance = vk::createInstance(createInfo);
    } catch (vk::SystemError &err) {
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
        throw std::runtime_error("Failed to find GPU device with Vulkan Support");
    }

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (!m_PhysicalDevice) {
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

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

    createInfo.enabledLayerCount = 0;

    m_Device = m_PhysicalDevice.createDevice(createInfo);
}

bool VulkanContext::IsDeviceSuitable(vk::PhysicalDevice device) const {
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(device, m_Surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.geometryShader;
}

bool VulkanContext::CheckDeviceExtensionSupport(vk::PhysicalDevice device) const {
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VulkanContext::QueueFamilyIndices VulkanContext::FindQueueFamilies(vk::PhysicalDevice device) const {
    QueueFamilyIndices indices;
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
    AR_CORE_INFO("Surface created.");
}




