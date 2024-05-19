#pragma once

#include "RendererType.h"
#include "Renderer.h"
#include "CommandBuffer.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "Sampler.h"
#include "Mesh.h"

class GraphicsFactory {
public:
    static Renderer* CreateRenderer(RendererType type);
    static CommandBuffer* CreateCommandBuffer(RendererType type);
    static Buffer* CreateBuffer(RendererType type);
    static Shader* CreateShader(RendererType type);
    static Texture* CreateTexture(RendererType type);
    static Material* CreateMaterial(RendererType type);
    static Pipeline* CreatePipeline(RendererType type);
    static RenderPass* CreateRenderPass(RendererType type);
    static FrameBuffer* CreateFrameBuffer(RendererType type);
    static Sampler* CreateSampler(RendererType type);
    static Mesh* CreateMesh(RendererType type);
};
