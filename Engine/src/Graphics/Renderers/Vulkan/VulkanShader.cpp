#include "VulkanShader.h"
#include "VulkanRenderer.h"

VulkanShader::VulkanShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    Load(vertexPath, fragmentPath);
}


VulkanShader::~VulkanShader() {

}

void VulkanShader::Bind() const {
    // Vulkan-specific bind implementation

}

void VulkanShader::Unbind() const {
    // Vulkan-specific unbind implementation
}

void VulkanShader::Load(const std::string& vertexPath, const std::string& fragmentPath) {
    // Load and create shader modules
}

void VulkanShader::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
    // Vulkan-specific shader module creation
}
