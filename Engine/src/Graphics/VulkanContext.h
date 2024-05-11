#pragma once
#include "vulkan//vulkan.h"

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    void Initialize();
    VkDevice GetDevice();
private:
    VkInstance m_Instance;
    VkDevice m_Device;
    //ToDo: Add vulkan specific members

    void CreateInstance();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
};
