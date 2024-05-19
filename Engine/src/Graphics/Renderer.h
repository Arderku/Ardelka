#pragma once

//class Entity;
class Mesh;
class CommandBuffer;
class Material;

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void Init() = 0;
    virtual void Render() =0; //ToDo this may change (Entity* rootEntity) = 0;
    virtual void Shutdown() = 0;
    virtual CommandBuffer* CreateCommandBuffer() = 0;
};
