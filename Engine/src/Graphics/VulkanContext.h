#pragma once
#include "Ardelkapch.h"
#include "vulkan/vulkan.hpp"

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    void Initialize();
    vk::Device GetDevice();
private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value();  //ToDo && presentFamily.has_value();
        }
    };

    vk::Instance m_Instance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    //ToDo: Add vulkan specific members

    void CreateInstance();
    static std::vector<const char*> GetRequiredExtensions();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);
    bool IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers);
};
