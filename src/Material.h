#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

class Material {
public:
    Material(Shader* shader);

    void setBaseColor(const glm::vec3& color);
    void setMetallic(float metallic);
    void setRoughness(float roughness);
    void setAlbedoMap(Texture* texture);
    void setMetallicMap(Texture* texture);
    void setRoughnessMap(Texture* texture);
    void setNormalMap(Texture* texture);

    void bind() const;

    Shader* GetShader() const;

private:
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
