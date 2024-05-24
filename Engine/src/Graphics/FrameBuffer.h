#pragma once

#include <memory>

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static std::shared_ptr<FrameBuffer> Create();
};
