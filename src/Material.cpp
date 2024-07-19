#include "Material.h"
//#include "stb_image.h"

Material::Material(Shader* shader)
        : m_Shader(shader), m_BaseColor(1.0f), m_Metallic(0.0f), m_Roughness(1.0f),
          m_AlbedoMap(0), m_MetallicMap(0), m_RoughnessMap(0), m_NormalMap(0) {}

void Material::setBaseColor(const glm::vec3& color) {
    m_BaseColor = color;
}

void Material::setMetallic(float metallic) {
    m_Metallic = metallic;
}

void Material::setRoughness(float roughness) {
    m_Roughness = roughness;
}

void Material::setAlbedoMap(unsigned int texture) {
    m_AlbedoMap = texture;
}

void Material::setMetallicMap(unsigned int texture) {
    m_MetallicMap = texture;
}

void Material::setRoughnessMap(unsigned int texture) {
    m_RoughnessMap = texture;
}

void Material::setNormalMap(unsigned int texture) {
    m_NormalMap = texture;
}

void Material::bind() const {
    m_Shader->use();
    m_Shader->setVec3("baseColor", m_BaseColor);
    m_Shader->setFloat("metallicValue", m_Metallic);
    m_Shader->setFloat("roughnessValue", m_Roughness);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_AlbedoMap);
    m_Shader->setInt("albedoMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_MetallicMap);
    m_Shader->setInt("metallicMap", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_RoughnessMap);
    m_Shader->setInt("roughnessMap", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_NormalMap);
    m_Shader->setInt("normalMap", 3);
}

Shader* Material::GetShader() const {
    return m_Shader;
}
