#include "Shader.h"
#include "Renderer.h"
#include "Graphics/Renderers/Vulkan/VulkanShader.h"

std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath) {
    switch (Renderer::GetAPI()) {
        case GraphicsAPI::None:
            throw std::runtime_error("GraphicsAPI::None is currently not supported!");
        case GraphicsAPI::Vulkan:
            return std::make_shared<VulkanShader>(vertexPath, fragmentPath);
            // Add cases for other APIs
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}

