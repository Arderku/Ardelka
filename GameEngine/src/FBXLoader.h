#pragma once
#include <memory>
#include <string>
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class FBXLoader {
public:
    static std::unique_ptr<GameObject> LoadModel(const std::string& path, Shader* shader);

private:
    static void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent);
    static std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static std::unique_ptr<Material> ProcessMaterial(aiMaterial* material, const aiScene* scene);
    static std::string GetTexturePath(aiMaterial* material, aiTextureType type);
    static Shader* m_Shader;
};
