#pragma once

#include <memory>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

class FBXLoader {
public:
    static std::unique_ptr<GameObject> LoadModel(const std::string& path, Shader* shader);
private:
    static void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent);
    static std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static std::unique_ptr<Material> ProcessMaterial(aiMaterial* material, const aiScene* scene);
    static std::string ExtractModelName(const std::string& path);

    static Shader* m_Shader;
};
