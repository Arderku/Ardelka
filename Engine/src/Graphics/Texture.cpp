#include "Texture.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanTexture.h"

std::shared_ptr<Texture> Texture::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanTexture>(filepath);
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
