#include "ResourceManager.h"
#include "FBXLoader.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;
std::unordered_map<std::string, std::unique_ptr<GameObject>> ResourceManager::m_Models;
std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders;
std::unordered_map<std::string, std::shared_ptr<Material>> ResourceManager::m_Materials;

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
    m_Textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name) {
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }
    return nullptr;
}

std::unique_ptr<GameObject> ResourceManager::loadModel(const std::string& path, std::shared_ptr<Shader> shader) {
    auto model = FBXLoader::LoadModel(path, shader);
    return model;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    if (m_Shaders.find(name) != m_Shaders.end()) {
        return m_Shaders[name];
    }

    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    m_Shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name) {
    if (m_Shaders.find(name) != m_Shaders.end()) {
        return m_Shaders[name];
    }
    return nullptr;
}

std::shared_ptr<Material> ResourceManager::createMaterial(const std::string& name, std::shared_ptr<Shader> shader) {
    if (m_Materials.find(name) != m_Materials.end()) {
        return m_Materials[name];
    }

    auto material = std::make_shared<Material>(name, shader);
    m_Materials[name] = material;
    return material;
}

std::shared_ptr<Material> ResourceManager::getMaterial(const std::string& name) {
    if (m_Materials.find(name) != m_Materials.end()) {
        return m_Materials[name];
    }
    return nullptr;
}

void ResourceManager::clear() {
    m_Textures.clear();
    m_Models.clear();
    m_Shaders.clear();
    m_Materials.clear();
}
