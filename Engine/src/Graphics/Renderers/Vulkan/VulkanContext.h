#pragma once
#include "Ardelkapch.h"
#include "Core/Window.h"
#include "vulkan/vulkan.hpp"
#include <vector>
#include <optional>

class VulkanContext {
public:
    explicit VulkanContext(Window& window);
    ~VulkanContext();

    void Initialize();
    void Cleanup();
    Window& GetWindow() const;
    vk::Device GetDevice() const;
    vk::PhysicalDevice GetPhysicalDevice() const;
    vk::Instance GetInstance() const;
    vk::SurfaceKHR GetSurface() const;
    uint32_t GetGraphicsQueueFamilyIndex() const;
    vk::Queue GetGraphicsQueue() const;
    vk::Queue GetPresentQueue() const;



    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily = std::nullopt;
        std::optional<uint32_t> presentFamily = std::nullopt;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device) const;



private:
    Window& m_Window;
    bool m_EnableValidationLayers = true;
    vk::Instance m_Instance = VK_NULL_HANDLE;
    vk::PhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    vk::Device m_Device = VK_NULL_HANDLE;
    vk::SurfaceKHR m_Surface = VK_NULL_HANDLE;

    std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    vk::DebugUtilsMessengerEXT m_DebugMessenger;

    void CreateInstance();
    void SetupDebugMessenger();
    std::vector<const char*> GetRequiredExtensions();
    std::vector<VkLayerProperties> GetSupportedValidationLayers();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();
    bool IsDeviceSuitable(vk::PhysicalDevice device) const;
    bool CheckDeviceExtensionSupport(vk::PhysicalDevice device) const;

};
