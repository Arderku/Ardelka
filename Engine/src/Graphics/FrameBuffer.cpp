#include "FrameBuffer.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanFrameBuffer.h"

std::shared_ptr<FrameBuffer> FrameBuffer::Create() {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanFrameBuffer>();
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
