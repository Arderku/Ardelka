#include "VulkanContext.h"
#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

VulkanContext::VulkanContext(Window& window) : m_Window(window) {}

VulkanContext::~VulkanContext() {
    //maybe destory it in engine?? will return to this
    m_Instance.destroy();
}

void VulkanContext::Initialize() {
    CreateInstance();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();

}

vk::Device VulkanContext::GetDevice() {
    return m_Device;
}

void VulkanContext::CreateInstance() {
    uint32_t version = 0;

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
        0, nullptr, // these prob should have glfw layers later
        extensions.size(), extensions.data());

    try {
        m_Instance = vk::createInstance(createInfo);
    }
    catch (vk::SystemError &err) {
        std::cout << "Failed to create Vulkan instance!" << std::endl;
    }
}

std::vector<const char*> VulkanContext::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    std::cout << "GLFW required extensions: " + std::to_string(glfwExtensionCount) << std::endl;
    return extensions;
}

void VulkanContext::PickPhysicalDevice() {
    std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();

    if (devices.empty()) {
        std::cout << "Failed to find GPU device with Vulkan Support" << std::endl;
    }

    for (const auto& device : devices) {
        vk::PhysicalDeviceProperties properties = device.getProperties();
        std::cout << "Physical Device: " << properties.deviceName << std::endl;
    }

    m_PhysicalDevice = devices[0];
}

void VulkanContext::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    if (!indices.isComplete()) {
        throw std::runtime_error("Failed to find a complete set of queue families");
    }

    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo({ }, indices.graphicsFamily.value(), 1, &queuePriority);
    vk::DeviceCreateInfo createInfo({}, 1, &queueCreateInfo);

    m_Device = m_PhysicalDevice.createDevice(createInfo);
    m_GraphicsQueue = m_Device.getQueue(indices.graphicsFamily.value(), 0);
    m_PresentQueue = m_Device.getQueue(indices.presentFamily.value(), 0);

    //print out graphics queue and present queue
    std::cout << "Graphics Queue: " << m_GraphicsQueue << std::endl;
    std::cout << "Present Queue: " << m_PresentQueue << std::endl;
    
    //print out device properties
    vk::PhysicalDeviceProperties properties = m_PhysicalDevice.getProperties();
    std::cout << "Logical Device: " << properties.deviceName << std::endl;
}

bool VulkanContext::IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers) {
    // Get the supported extensions
    std::vector<vk::ExtensionProperties> availableExtensions = m_PhysicalDevice.enumerateDeviceExtensionProperties();

    // Check if all required extensions are supported
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

    // Check if all required features are supported
    vk::PhysicalDeviceFeatures supportedFeatures = m_PhysicalDevice.getFeatures();
    // Add checks for required features here. For example:
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




