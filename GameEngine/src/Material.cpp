#include "Material.h"
#include "Shader.h"

Material::Material(const std::string& name, std::shared_ptr<Shader> shader)
        : m_Name(name), m_Shader(shader), m_BaseColor(glm::vec3(1.0f)), m_Metallic(0.0f), m_Roughness(1.0f) {}

void Material::SetBaseColor(const glm::vec3& color) {
    m_BaseColor = color;
}

void Material::SetMetallic(float metallic) {
    m_Metallic = metallic;
}

void Material::SetRoughness(float roughness) {
    m_Roughness = roughness;
}

void Material::SetAlbedoMap(std::shared_ptr<Texture> texture) {
    m_AlbedoMap = texture;
}

void Material::SetMetallicMap(std::shared_ptr<Texture> texture) {
    m_MetallicMap = texture;
}

void Material::SetRoughnessMap(std::shared_ptr<Texture> texture) {
    m_RoughnessMap = texture;
}

void Material::SetNormalMap(std::shared_ptr<Texture> texture) {
    m_NormalMap = texture;
}

std::shared_ptr<Texture> Material::GetAlbedoMap() const {
    return m_AlbedoMap;
}

std::shared_ptr<Texture> Material::GetMetallicMap() const {
    return m_MetallicMap;
}

std::shared_ptr<Texture> Material::GetRoughnessMap() const {
    return m_RoughnessMap;
}

std::shared_ptr<Texture> Material::GetNormalMap() const {
    return m_NormalMap;
}

void Material::Bind() const {
    m_Shader->use();
    m_Shader->setVec3("baseColor", m_BaseColor);
    m_Shader->setFloat("metallic", m_Metallic);
    m_Shader->setFloat("roughness", m_Roughness);

    if (m_AlbedoMap) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_AlbedoMap->GetID());
        m_Shader->setInt("albedoMap", 0);
    }
    if (m_MetallicMap) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_MetallicMap->GetID());
        m_Shader->setInt("metallicMap", 1);
    }
    if (m_RoughnessMap) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_RoughnessMap->GetID());
        m_Shader->setInt("roughnessMap", 2);
    }
    if (m_NormalMap) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_NormalMap->GetID());
        m_Shader->setInt("normalMap", 3);
    }
}

void Material::Unbind() const {
    // Optionally unbind textures and shader
}

std::shared_ptr<Shader> Material::GetShader() const {
    return m_Shader;
}

const std::string &Material::GetName() const {
    return m_Name;
}
