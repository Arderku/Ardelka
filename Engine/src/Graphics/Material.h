#pragma once

class Material {
public:
    virtual ~Material() = default;
    virtual void Bind() const = 0;
};
