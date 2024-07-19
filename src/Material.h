#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "Shader.h"

class Material {
public:
    Material(Shader* shader);

    void setBaseColor(const glm::vec3& color);
    void setMetallic(float metallic);
    void setRoughness(float roughness);

    void setAlbedoMap(unsigned int texture);
    void setMetallicMap(unsigned int texture);
    void setRoughnessMap(unsigned int texture);
    void setNormalMap(unsigned int texture);

    Shader* GetShader() const;

    void bind() const;


private:
    Shader* m_Shader;
    glm::vec3 m_BaseColor;
    float m_Metallic;
    float m_Roughness;
    unsigned int m_AlbedoMap;
    unsigned int m_MetallicMap;
    unsigned int m_RoughnessMap;
    unsigned int m_NormalMap;
};

#endif // MATERIAL_H
