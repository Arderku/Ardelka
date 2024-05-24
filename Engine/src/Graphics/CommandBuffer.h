#pragma once

#include <memory>

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;
    virtual void Submit() = 0;

    static std::shared_ptr<CommandBuffer> Create();
};
