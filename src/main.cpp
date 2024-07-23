#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Camera.h"

int main() {
    Engine engine;
    engine.Init();
    std::cerr << "Starting main" << std::endl;

    std::cerr << "Creating Shader" << std::endl;

    engine.GetRenderer().GetShader();

    // Create a GameObject with a Camera component
    auto cameraGameObject = std::make_unique<GameObject>();
    cameraGameObject->SetName("MainCamera");
    cameraGameObject->AddComponent(std::make_unique<Camera>(60.0f, 16.0f/9.0f, 0.1f, 1000.0f));
    cameraGameObject->GetTransform()->position = glm::vec3(0.0f, 0.0f, -5.0f);

    engine.GetScene().SetActiveCamera(cameraGameObject->GetComponent<Camera>());
    engine.GetScene().AddGameObject(std::move(cameraGameObject));

    std::cerr << "Creating Material" << std::endl;
    Texture* albedo = ResourceManager::loadTexture("albedo", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_albedo.png");
    Texture* metallic = ResourceManager::loadTexture("metallic", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_metallic.png");
    Texture* roughness = ResourceManager::loadTexture("roughness", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_roughness.png");
    Texture* normal = ResourceManager::loadTexture("normal", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_normal-dx.png");

    Material* materialRed = new Material("M_Red",engine.GetRenderer().GetShader());
    materialRed->SetBaseColor(glm::vec4(1.5f, 0.5f, 0.5f, 1.0f));
    materialRed->SetBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    materialRed->SetMetallic(0.5f);
    materialRed->SetRoughness(0.15f);

    materialRed->SetAlbedoMap(albedo);
    materialRed->SetMetallicMap(metallic);
    materialRed->SetRoughnessMap(roughness);
    materialRed->SetNormalMap(normal);

    Material* materialBlue = new Material("M_Blue",engine.GetRenderer().GetShader());
    materialBlue->SetBaseColor(glm::vec4(.0f, 0.0f, 1.0f, 1.0f));
    materialBlue->SetNormalMap(normal);

    // Define the cube vertices and indices
    std::vector<float> vertices = {
            // positions          // normals           // texcoords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,      // Front face
            4, 5, 6, 6, 7, 4,      // Back face
            8, 9, 10, 10, 11, 8,   // Left face
            12, 13, 14, 14, 15, 12,// Right face
            16, 17, 18, 18, 19, 16,// Bottom face
            20, 21, 22, 22, 23, 20 // Top face
    };

    std::cerr << "Creating Mesh" << std::endl;
    Mesh* mesh = new Mesh(vertices, indices);

    std::cerr << "Creating parent GameObject" << std::endl;
    auto parentGameObject = std::make_unique<GameObject>();
    parentGameObject->GetTransform()->position = glm::vec3(0.0f, 0.0f, 0.0f);
    //scale the object
    parentGameObject->GetTransform()->scale = glm::vec3(1);
    parentGameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialRed));

    std::cerr << "Creating child GameObject" << std::endl;
    auto childGameObject = std::make_unique<GameObject>();
    childGameObject->SetName("BlueCube");
    childGameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialBlue));
    childGameObject->GetTransform()->position = glm::vec3(2.0f, 1.0f, 0.0f);
    childGameObject->GetTransform()->scale = glm::vec3(2.f, 1.f, 0.5f);

    parentGameObject->AddChild(std::move(childGameObject));

    std::cerr << "Adding GameObject to scene" << std::endl;
    engine.GetScene().AddGameObject(std::move(parentGameObject));

    //creat 100 game objects at random positions
    for (int i = 0; i < 1000; i++) {
        auto gameObject = std::make_unique<GameObject>();
        gameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialRed));
        gameObject->GetTransform()->position = glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5,
                                                         static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5,
                                                         static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5);
        gameObject->GetTransform()->scale = glm::vec3(0.5f);
        engine.GetScene().AddGameObject(std::move(gameObject));
    }

    std::cerr << "Starting engine run" << std::endl;
    engine.Run();
    return 0;
}
