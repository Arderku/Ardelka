#include "ResourceManager.h"
#include "FBXLoader.h"

std::unordered_map<std::string, Texture*> ResourceManager::m_Textures;
std::unordered_map<std::string, std::unique_ptr<GameObject>> ResourceManager::m_Models;

Texture* ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }

    Texture* texture = new Texture(path);
    m_Textures[name] = texture;
    return texture;
}

Texture* ResourceManager::getTexture(const std::string& name) {
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }
    return nullptr;
}

std::unique_ptr<GameObject> ResourceManager::loadModel(const std::string& path, Shader* shader) {
    auto model = FBXLoader::LoadModel(path, shader);
    return model;
}

void ResourceManager::clear() {
    for (auto& pair : m_Textures) {
        delete pair.second;
    }
    m_Textures.clear();
    m_Models.clear();
}
