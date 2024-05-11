#include "VulkanContext.h"

VulkanContext::VulkanContext() {
}

VulkanContext::~VulkanContext() {
    //maybe destory it in engine?? will return to this
    m_Instance.destroy();
}

void VulkanContext::Initialize() {
    CreateInstance();
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
        "Vulkan Engine",
        version,
        "Graphics Engine Dev",
        version,
        version
        );

    auto createInfo = vk::InstanceCreateInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        0, nullptr,
        0, nullptr); // these prob should have glfw layers later

    try {
        m_Instance = vk::createInstance(createInfo);
    }
    catch (vk::SystemError &err) {
        std::cout << "Failed to create Vulkan instance!" << std::endl;
    }
}

void VulkanContext::PickPhysicalDevice() {
    std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();

    if (devices.empty()) {
        std::cout << "Failed to find GPU device with Vulkan Support" << std::endl;
    }

    // temp select the first device 1st
    m_PhysicalDevice = devices[0];
}

void VulkanContext::CreateLogicalDevice() {
    //temp to be extended
    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo({}, 0, 1, &queuePriority);
    vk::DeviceCreateInfo createInfo({}, 1, &queueCreateInfo);

    m_Device = m_PhysicalDevice.createDevice(createInfo);
}
