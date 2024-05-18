#pragma once
#include "Ardelkapch.h"
#include "VulkanContext.h"
#include "vulkan/vulkan.hpp"

class SwapChain {
public:
    SwapChain(VulkanContext& context);
    ~SwapChain();

    void Initialize();
    void Cleanup();
    vk::SwapchainKHR GetSwapChain() const;
    vk::Format GetImageFormat() const;
    const std::vector<vk::ImageView>& GetImageViews() const;
    vk::Extent2D GetExtent() const;


    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;

        bool IsValid() const { return !formats.empty() && !presentModes.empty(); }
    };

    static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);

private:
    VulkanContext& m_Context;
    vk::SwapchainKHR m_SwapChain = VK_NULL_HANDLE;
    std::vector<vk::Image> m_SwapChainImages;
    std::vector<vk::ImageView> m_SwapChainImageViews;
    vk::Format m_SwapChainImageFormat;
    vk::Extent2D m_SwapChainExtent;

    void CreateSwapChain();
    void CreateImageViews();
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const;
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;
};
