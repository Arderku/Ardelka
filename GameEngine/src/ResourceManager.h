#pragma once
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "GameObject.h"
#include "Shader.h"

class ResourceManager {
public:
    static Texture* loadTexture(const std::string& name, const std::string& path);
    static Texture* getTexture(const std::string& name);
    static std::unique_ptr<GameObject> loadModel(const std::string& path, Shader* shader);
    static void clear();

private:
    static std::unordered_map<std::string, Texture*> m_Textures;
    static std::unordered_map<std::string, std::unique_ptr<GameObject>> m_Models;
};
