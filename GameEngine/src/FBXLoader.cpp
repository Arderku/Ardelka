#include "FBXLoader.h"
#include <iostream>
#include <filesystem>
#include "ResourceManager.h"
#include "MeshRenderer.h"

Shader* FBXLoader::m_Shader = nullptr;

std::unique_ptr<GameObject> FBXLoader::LoadModel(const std::string& path, Shader* shader) {
    m_Shader = shader;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::string modelName = ExtractModelName(path);
    std::cerr << "Processing root node: " << scene->mRootNode->mName.C_Str() << std::endl;
    auto rootGameObject = std::make_unique<GameObject>();
    rootGameObject->SetName(modelName);
    ProcessNode(scene->mRootNode, scene, rootGameObject.get());

    return rootGameObject;
}

void FBXLoader::ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent) {
    bool isPivotNode = std::string(node->mName.C_Str()).find("$AssimpFbx$") != std::string::npos;
    std::cerr << "Processing node: " << node->mName.C_Str() << " with " << node->mNumChildren << " children." << std::endl;

    if (!isPivotNode && node->mNumMeshes > 0) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            auto meshComponent = ProcessMesh(mesh, scene);
            auto materialComponent = ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex], scene);
            parent->AddComponent(std::make_unique<MeshRenderer>(meshComponent.release(), materialComponent.release(), m_Shader));
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, parent);
    }
}

std::unique_ptr<Mesh> FBXLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mNormals) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y); // Do not flip texture coordinates
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Material> FBXLoader::ProcessMaterial(aiMaterial* material, const aiScene* scene) {
    auto mat = std::make_unique<Material>("BasicMaterial", m_Shader);

    aiColor3D color(0.f, 0.f, 0.f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mat->SetBaseColor(glm::vec3(color.r, color.g, color.b));

    float metallic = 0.0f;
    material->Get(AI_MATKEY_REFLECTIVITY, metallic);
    mat->SetMetallic(metallic);

    float roughness = 0.0f;
    material->Get(AI_MATKEY_SHININESS, roughness);
    mat->SetRoughness(roughness);

    // Add support for textures here

    return mat;
}

std::string FBXLoader::ExtractModelName(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}
