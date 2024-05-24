#include "CommandBuffer.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanCommandBuffer.h"

std::shared_ptr<CommandBuffer> CommandBuffer::Create() {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanCommandBuffer>();
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
