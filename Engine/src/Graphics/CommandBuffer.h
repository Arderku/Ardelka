#pragma once

#include <cstdint>

class Buffer;
class Shader;
class Material;

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;
    virtual void Begin() = 0;
    virtual void End() = 0;
    virtual void BindVertexBuffer(Buffer* buffer) = 0;
    virtual void BindIndexBuffer(Buffer* buffer) = 0;
    virtual void BindShader(Shader* shader) = 0;
    virtual void BindMaterial(Material* material) = 0;
    virtual void DrawIndexed(uint32_t indexCount) = 0;
};
