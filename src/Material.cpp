#include "Material.h"

Material::Material(Shader* shader)
        : m_Shader(shader), m_BaseColor(1.0f), m_Metallic(0.0f), m_Roughness(1.0f),
          m_AlbedoMap(nullptr), m_MetallicMap(nullptr), m_RoughnessMap(nullptr), m_NormalMap(nullptr) {}

void Material::setBaseColor(const glm::vec3& color) {
    m_BaseColor = color;
}

void Material::setMetallic(float metallic) {
    m_Metallic = metallic;
}

void Material::setRoughness(float roughness) {
    m_Roughness = roughness;
}

void Material::setAlbedoMap(Texture* texture) {
    m_AlbedoMap = texture;
}

void Material::setMetallicMap(Texture* texture) {
    m_MetallicMap = texture;
}

void Material::setRoughnessMap(Texture* texture) {
    m_RoughnessMap = texture;
}

void Material::setNormalMap(Texture* texture) {
    m_NormalMap = texture;
}

void Material::bind() const {
    m_Shader->use();
    m_Shader->setVec3("baseColor", m_BaseColor);
    m_Shader->setFloat("metallicValue", m_Metallic);
    m_Shader->setFloat("roughnessValue", m_Roughness);


    if (m_AlbedoMap) {
        m_AlbedoMap->bind(0);
        m_Shader->setInt("albedoMap", 0);
    }

    if (m_MetallicMap) {
        m_MetallicMap->bind(1);
        m_Shader->setInt("metallicMap", 1);
    }

    if (m_RoughnessMap) {
        m_RoughnessMap->bind(2);
        m_Shader->setInt("roughnessMap", 2);
    }

    if (m_NormalMap) {
        m_NormalMap->bind(3);
        m_Shader->setInt("normalMap", 3);
    }
}

Shader* Material::GetShader() const {
    return m_Shader;
}
