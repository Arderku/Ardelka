#pragma once

#include <memory>

class Buffer {
public:
    virtual ~Buffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static std::shared_ptr<Buffer> Create(size_t size, const void* data);
};
