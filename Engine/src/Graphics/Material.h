#pragma once

class Shader;
class Texture;

class Material {
public:
    virtual ~Material() = default;
    virtual void Bind() const = 0;
    virtual void SetShader(Shader* shader) = 0;
    virtual void SetTexture(const std::string& name, Texture* texture) = 0;
};
