#include "Material.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanMaterial.h"

std::shared_ptr<Material> Material::Create() {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanMaterial>();
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
