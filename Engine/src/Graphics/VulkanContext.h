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
        std::optional<uint32_t> graphicsFamily = std::nullopt;
        std::optional<uint32_t> presentFamily = std::nullopt;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainProperties {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;

        bool IsValid() const { return !formats.empty() && !present_modes.empty(); }
    };

    Window& m_Window;
    vk::Instance m_Instance = VK_NULL_HANDLE;
    vk::PhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    vk::Device m_Device = VK_NULL_HANDLE;
    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue = VK_NULL_HANDLE;

    vk::SurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkSurfaceFormatKHR m_SurfaceFormat;

    VkSwapchainKHR m_SwapChain_ = VK_NULL_HANDLE;
    VkPresentModeKHR m_PresentMode;
    //ToDo: Add vulkan specific members

    void CreateInstance();
    static std::vector<const char*> GetRequiredExtensions();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();

    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);
    bool IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers);
};
