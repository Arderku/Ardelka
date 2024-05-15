#pragma once
#include "Ardelkapch.h"
#include "Core/Window.h"
#include "vulkan/vulkan.hpp"

class VulkanContext {
public:
    explicit VulkanContext(Window& window);
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

    Window& m_Window;
    vk::Instance m_Instance = VK_NULL_HANDLE;
    vk::PhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    vk::Device m_Device = VK_NULL_HANDLE;
    vk::SurfaceKHR m_Surface = VK_NULL_HANDLE;
    //ToDo: Add vulkan specific members

    void CreateInstance();
    static std::vector<const char*> GetRequiredExtensions();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();

    static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);
    bool IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers);
};
