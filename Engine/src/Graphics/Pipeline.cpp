#include "Pipeline.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanPipeline.h"

std::shared_ptr<Pipeline> Pipeline::Create() {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanPipeline>();
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
