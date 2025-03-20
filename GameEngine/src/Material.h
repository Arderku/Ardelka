#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Material {
public:
    Material(const std::string& name, std::shared_ptr<Shader> shader);

    void SetBaseColor(const glm::vec3& color);
    void SetMetallic(float metallic);
    void SetRoughness(float roughness);
    void SetAlbedoMap(std::shared_ptr<Texture> texture);
    void SetMetallicMap(std::shared_ptr<Texture> texture);
    void SetRoughnessMap(std::shared_ptr<Texture> texture);
    void SetNormalMap(std::shared_ptr<Texture> texture);

    glm::vec3 GetBaseColor() const { return m_BaseColor; }
    float GetMetallic() const { return m_Metallic; }
    float GetRoughness() const { return m_Roughness; }

    std::shared_ptr<Texture> GetAlbedoMap() const;
    std::shared_ptr<Texture> GetMetallicMap() const;
    std::shared_ptr<Texture> GetRoughnessMap() const;
    std::shared_ptr<Texture> GetNormalMap() const;

    void Bind() const;
    void Unbind() const;

    std::shared_ptr<Shader> GetShader() const;

    const std::string& GetName() const;

private:
    std::string m_Name;

    std::shared_ptr<Shader> m_Shader;
    glm::vec3 m_BaseColor;
    float m_Metallic;
    float m_Roughness;
    std::shared_ptr<Texture> m_AlbedoMap;
    std::shared_ptr<Texture> m_MetallicMap;
    std::shared_ptr<Texture> m_RoughnessMap;
    std::shared_ptr<Texture> m_NormalMap;
};

#endif // MATERIAL_H
