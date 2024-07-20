#include "ResourceManager.h"

std::unordered_map<std::string, Texture*> ResourceManager::m_Textures;

Texture* ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    // Check if texture is already loaded
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }

    // Load new texture and store it
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

void ResourceManager::clear() {
    for (auto& pair : m_Textures) {
        delete pair.second;
    }
    m_Textures.clear();
}
