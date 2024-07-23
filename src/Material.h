#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

class Material {
public:
    Material(const std::string& name, Shader* shader);

    void SetBaseColor(const glm::vec3& color);
    void SetMetallic(float metallic);
    void SetRoughness(float roughness);
    void SetAlbedoMap(Texture* texture);
    void SetMetallicMap(Texture* texture);
    void SetRoughnessMap(Texture* texture);
    void SetNormalMap(Texture* texture);

    glm::vec3 GetBaseColor() const { return m_BaseColor; }
    float GetMetallic() const { return m_Metallic; }
    float GetRoughness() const { return m_Roughness; }

    Texture* GetAlbedoMap() const;
    Texture* GetMetallicMap() const;
    Texture* GetRoughnessMap() const;
    Texture* GetNormalMap() const;


    void Bind() const;

    Shader* GetShader() const;

    void Unbind() const;

    const std::string& GetName() const;

private:
    std::string m_Name;

    Shader* m_Shader;
    glm::vec3 m_BaseColor;
    float m_Metallic;
    float m_Roughness;
    Texture* m_AlbedoMap;
    Texture* m_MetallicMap;
    Texture* m_RoughnessMap;
    Texture* m_NormalMap;
};

#endif // MATERIAL_H
