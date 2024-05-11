#pragma once
#include "vulkan/vulkan.hpp"

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    void Initialize();
    vk::Device GetDevice();
private:
    vk::Instance m_Instance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    //ToDo: Add vulkan specific members

    void CreateInstance();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
};
