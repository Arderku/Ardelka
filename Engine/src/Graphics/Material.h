#pragma once

#include <memory>

class Shader;
class Texture;

class Material {
public:
    virtual ~Material() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static std::shared_ptr<Material> Create();

    virtual void SetShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetTexture(const std::shared_ptr<Texture>& texture) = 0;
};
