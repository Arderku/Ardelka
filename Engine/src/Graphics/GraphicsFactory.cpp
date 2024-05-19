#include "GraphicsFactory.h"
/*#include "Renderers/Vulkan/VulkanRenderer.h"
#include "Renderers/Vulkan/VulkanCommandBuffer.h"
#include "Renderers/Vulkan/VulkanBuffer.h"
#include "Renderers/Vulkan/VulkanShader.h"
#include "Renderers/Vulkan/VulkanTexture.h"
#include "Renderers/Vulkan/VulkanMaterial.h"
#include "Renderers/Vulkan/VulkanPipeline.h"
#include "Renderers/Vulkan/VulkanRenderPass.h"*/
// Include DirectX, OpenGL, WebGL headers similarly

Renderer* GraphicsFactory::CreateRenderer(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanRenderer();
        default:
            return nullptr;
    }
}

CommandBuffer* GraphicsFactory::CreateCommandBuffer(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanCommandBuffer();
        default:
            return nullptr;
    }
}

Buffer* GraphicsFactory::CreateBuffer(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanBuffer();
        default:
            return nullptr;
    }
}

Shader* GraphicsFactory::CreateShader(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanShader();
        default:
            return nullptr;
    }
}

Texture* GraphicsFactory::CreateTexture(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanTexture();
        default:
            return nullptr;
    }
}

Material* GraphicsFactory::CreateMaterial(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanMaterial();
        default:
            return nullptr;
    }
}

Pipeline* GraphicsFactory::
CreatePipeline(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanPipeline();
        default:
            return nullptr;
    }
}

RenderPass* GraphicsFactory::CreateRenderPass(RendererType type) {
    switch (type) {
        case RendererType::Vulkan:
            return nullptr; //ToDo new VulkanRenderPass();
        default:
            return nullptr;
    }
}

#include "GraphicsFactory.h"
