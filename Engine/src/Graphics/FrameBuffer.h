#pragma once

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual void Create() = 0;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    // Retrieves the ID or handle of the framebuffer (optional, for certain APIs)
    virtual unsigned int GetID() const = 0;
};
