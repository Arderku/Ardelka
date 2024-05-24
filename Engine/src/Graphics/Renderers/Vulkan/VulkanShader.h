#pragma once

#include "Graphics/Shader.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class VulkanShader : public Shader {
public:
    VulkanShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~VulkanShader();

    void Bind() const override = 0;
    void Unbind() const override = 0;
    void Load(const std::string& vertexPath, const std::string& fragmentPath) override;

private:
    VkDevice m_Device;
    VkPipeline m_Pipeline;
    VkPipelineLayout m_PipelineLayout;

    void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
};
