#pragma once
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "GameObject.h"
#include "Shader.h"
#include "Material.h"

class ResourceManager {
public:
    static std::shared_ptr<Texture> loadTexture(const std::string& name, const std::string& path);
    static std::shared_ptr<Texture> getTexture(const std::string& name);
    static std::shared_ptr<Texture> GetDefaultWhiteTexture();

    static std::unique_ptr<GameObject> loadModel(const std::string& path, std::shared_ptr<Shader> shader);

    static std::shared_ptr<Shader> loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Shader> getShader(const std::string& name);

    static std::shared_ptr<Material> createMaterial(const std::string& name, std::shared_ptr<Shader> shader);
    static std::shared_ptr<Material> getMaterial(const std::string& name);

    static void clear();

private:
    static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    static std::unordered_map<std::string, std::unique_ptr<GameObject>> m_Models;
    static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    static std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
};
