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
    vk::Device GetDevice();

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily = std::nullopt;
        std::optional<uint32_t> presentFamily = std::nullopt;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;

        bool IsValid() const { return !formats.empty() && !presentModes.empty(); }
    };

    Window& m_Window;
    vk::Instance m_Instance = VK_NULL_HANDLE;
    vk::PhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    vk::Device m_Device = VK_NULL_HANDLE;
    vk::Queue m_GraphicsQueue = VK_NULL_HANDLE;
    vk::Queue m_PresentQueue = VK_NULL_HANDLE;
    vk::SurfaceKHR m_Surface = VK_NULL_HANDLE;
    vk::SwapchainKHR m_SwapChain = VK_NULL_HANDLE;
    std::vector<vk::Image> m_SwapChainImages;
    vk::Format m_SwapChainImageFormat;
    vk::Extent2D m_SwapChainExtent;

    vk::DebugUtilsMessengerEXT m_DebugMessenger;

    void CreateInstance();
    static std::vector<const char*> GetRequiredExtensions();
    static std::vector<VkLayerProperties> GetSupportedValidationLayers();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapChain();

    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);
    bool IsDeviceSuitable(const std::vector<const char*>& extensions, std::vector<const char*>& layers);

    SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

};
