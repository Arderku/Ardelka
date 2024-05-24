#pragma once

#include <memory>

class Pipeline {
public:
    virtual ~Pipeline() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static std::shared_ptr<Pipeline> Create();
};
