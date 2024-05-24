#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanRenderer.h"

GraphicsAPI Renderer::s_GraphicsAPI = GraphicsAPI::None;

std::shared_ptr<Renderer> Renderer::Create(Window& window, GraphicsAPI api) {
    s_GraphicsAPI = api;

    switch (api) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
           // return std::make_shared<VulkanRenderer>(window);
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}

