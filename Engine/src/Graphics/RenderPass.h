#pragma once

class RenderPass {
public:
    virtual ~RenderPass() = default;
    virtual void Begin() = 0;
    virtual void End() = 0;
};
