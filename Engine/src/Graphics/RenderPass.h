#pragma once

class FrameBuffer;

class RenderPass {
public:
    virtual ~RenderPass() = default;
    virtual void Begin(FrameBuffer* framebuffer) = 0;
    virtual void End() = 0;
};
