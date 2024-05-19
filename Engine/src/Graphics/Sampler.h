#pragma once

class Sampler {
public:
    virtual ~Sampler() = default;

    virtual void Create() = 0;

    virtual void Bind(unsigned int slot) const = 0;

    virtual void Unbind() const = 0;
};
