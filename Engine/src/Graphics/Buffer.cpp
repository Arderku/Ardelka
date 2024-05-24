#include "Buffer.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanBuffer.h"

std::shared_ptr<Buffer> Buffer::Create(size_t size, const void* data) {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanBuffer>(size, data);
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
