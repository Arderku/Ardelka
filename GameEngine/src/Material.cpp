#include "Material.h"

Material::Material(const std::string& name, Shader* shader)
        : m_Name(name) ,
        m_Shader(shader), m_BaseColor(1.0f), m_Metallic(0.0f), m_Roughness(1.0f),
        m_AlbedoMap(nullptr), m_MetallicMap(nullptr), m_RoughnessMap(nullptr), m_NormalMap(nullptr) {}

void Material::SetBaseColor(const glm::vec3& color) {
    m_BaseColor = color;
}

void Material::SetMetallic(float metallic) {
    m_Metallic = metallic;
}

void Material::SetRoughness(float roughness) {
    m_Roughness = roughness;
}

void Material::SetAlbedoMap(Texture* texture) {
    m_AlbedoMap = texture;
}

void Material::SetMetallicMap(Texture* texture) {
    m_MetallicMap = texture;
}

void Material::SetRoughnessMap(Texture* texture) {
    m_RoughnessMap = texture;
}

void Material::SetNormalMap(Texture* texture) {
    m_NormalMap = texture;
}



void Material::Bind() const {
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

void Material::Unbind() const {
    m_AlbedoMap->unbind(0);
    m_MetallicMap->unbind(1);
    m_RoughnessMap->unbind(2);
    m_NormalMap->unbind(3);
}

const std::string &Material::GetName() const {
    return m_Name;
}

Texture *Material::GetAlbedoMap() const {
    return m_AlbedoMap;
}

Texture *Material::GetMetallicMap() const {
    return m_MetallicMap;
}

Texture *Material::GetRoughnessMap() const {
    return m_RoughnessMap;
}

Texture *Material::GetNormalMap() const {
    return m_NormalMap;
}
