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
    static std::unique_ptr<GameObject> LoadModel(const std::string& path, std::shared_ptr<Shader> shader);
private:
    static void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent, std::shared_ptr<Shader> shader);
    static std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static std::shared_ptr<Material> ProcessMaterial(aiMaterial* material, const aiScene* scene, std::shared_ptr<Shader> shader);
    static std::string ExtractModelName(const std::string& path);
};
