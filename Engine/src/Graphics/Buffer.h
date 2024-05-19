#pragma once

#include <cstddef>

class Buffer {
public:
    virtual ~Buffer() = default;
    virtual void Allocate(size_t size) = 0;
    virtual void Update(const void* data, size_t size) = 0;
    virtual void Bind() const = 0;
};
