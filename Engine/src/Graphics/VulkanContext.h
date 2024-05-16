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
    bool m_EnableValidationLayers = true;
    vk::Instance m_Instance = VK_NULL_HANDLE;
    vk::PhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    vk::Device m_Device = VK_NULL_HANDLE;
    vk::Queue m_GraphicsQueue = VK_NULL_HANDLE;
    vk::Queue m_PresentQueue = VK_NULL_HANDLE;
    vk::SurfaceKHR m_Surface = VK_NULL_HANDLE;
    vk::SwapchainKHR m_SwapChain = VK_NULL_HANDLE;
    std::vector<vk::Image> m_SwapChainImages;
    std::vector<vk::ImageView> m_SwapChainImageViews;
    vk::Format m_SwapChainImageFormat;
    vk::Extent2D m_SwapChainExtent;

    // Sync objects
    std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
    std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
    std::vector<vk::Fence> m_InFlightFences;
    size_t m_CurrentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    std::vector<const char*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<const char*> m_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    vk::DebugUtilsMessengerEXT m_DebugMessenger;


    void CreateInstance();
    void SetupDebugMessenger();
      std::vector<const char*> GetRequiredExtensions();
      std::vector<VkLayerProperties> GetSupportedValidationLayers();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateSyncObjects();

    QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);

    bool IsDeviceSuitable(vk::PhysicalDevice device);

    SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

    bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);


};
