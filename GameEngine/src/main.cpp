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
#include "TestObject.h"

int main() {
    Engine engine;
    engine.Init();
    std::cerr << "Starting main" << std::endl;

    // Load shaders
    auto pbrShader = ResourceManager::loadShader("PBR", "Resources/Shaders/pbr_vertex.glsl", "Resources/Shaders/pbr_fragment.glsl");
    auto shadowShader = ResourceManager::loadShader("Shadow", "Resources/Shaders/shadow_vertex.glsl", "Resources/Shaders/shadow_fragment.glsl");
    auto skyboxShader = ResourceManager::loadShader("Skybox", "Resources/Shaders/skybox_vertex.glsl", "Resources/Shaders/skybox_fragment.glsl");

    // Create materials
    auto materialRed = ResourceManager::createMaterial("M_Red", pbrShader);
    materialRed->SetBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    materialRed->SetMetallic(0.5f);
    materialRed->SetRoughness(0.15f);

    auto materialBlue = ResourceManager::createMaterial("M_Blue", pbrShader);
    materialBlue->SetBaseColor(glm::vec3(0.0f, 0.0f, 1.0f));

    // Load textures and set them to materials if needed
    auto albedo = ResourceManager::loadTexture("albedo", "Resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_albedo.png");
    auto metallic = ResourceManager::loadTexture("metallic", "Resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_metallic.png");
    auto roughness = ResourceManager::loadTexture("roughness", "Resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_roughness.png");
    auto normal = ResourceManager::loadTexture("normal", "Resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_normal_dx.png");

    materialRed->SetAlbedoMap(albedo);
    materialRed->SetMetallicMap(metallic);
    materialRed->SetRoughnessMap(roughness);
    materialRed->SetNormalMap(normal);

    // Create a GameObject with a Camera component
    auto cameraGameObject = std::make_unique<GameObject>();
    cameraGameObject->SetName("MainCamera");
    cameraGameObject->AddComponent(std::make_unique<Camera>(60.0f, 2.0f / 1.0f, 0.1f, 1000.0f));
    cameraGameObject->GetTransform()->position = glm::vec3(0.0f, 0.0f, -5.0f);

    engine.GetScene().SetActiveCamera(cameraGameObject->GetComponent<Camera>());
    engine.GetScene().AddGameObject(std::move(cameraGameObject));


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
    auto mesh = std::make_shared<Mesh>(vertices, indices);

    std::cerr << "Creating parent GameObject" << std::endl;
    auto parentGameObject = std::make_unique<GameObject>();
    parentGameObject->GetTransform()->position = glm::vec3(0.0f, 0.0f, 0.0f);
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

    for (int i = 0; i < 1000; i++) {
        auto gameObject = std::make_unique<TestObject>();
        gameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialRed));

        gameObject->GetTransform()->position = glm::vec3(
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 14 - 7,
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 14 - 7,
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 14 - 7
        );

        gameObject->GetTransform()->scale = glm::vec3(0.5f);

        engine.GetScene().AddGameObject(std::move(gameObject));
    }

    std::cerr << "Loading GameBoyClassic model" << std::endl;
    auto gameBoyClassic = ResourceManager::loadModel("Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic.fbx", pbrShader);

    auto GameBoyAlbedo = ResourceManager::loadTexture("GameBoyAlbedo", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_BaseColor.png");
    auto GameBoyMetallic = ResourceManager::loadTexture("GameBoyMetallic", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Metallic.png");
    auto GameBoyRoughness = ResourceManager::loadTexture("GameBoyRoughness", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Roughness.png");
    auto GameBoyNormal = ResourceManager::loadTexture("GameBoyNormal", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Normal.png");

    auto GameBoyMaterial = ResourceManager::createMaterial("GameBoyClassic", pbrShader);
    GameBoyMaterial->SetBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    GameBoyMaterial->SetAlbedoMap(GameBoyAlbedo);

    auto gameBoyRenderer = gameBoyClassic->GetComponent<MeshRenderer>();
    gameBoyRenderer->GetMaterial()->SetBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    gameBoyRenderer->GetMaterial()->SetAlbedoMap(GameBoyAlbedo);
    gameBoyRenderer->GetMaterial()->SetMetallicMap(GameBoyMetallic);
    gameBoyRenderer->GetMaterial()->SetNormalMap(GameBoyNormal);

    gameBoyClassic->GetTransform()->position = glm::vec3(0.0f, -1.20f, 0.0f);
    gameBoyClassic->GetTransform()->rotation = glm::vec3(0.0f, 45.0f, 0.0f);
    gameBoyClassic->GetTransform()->scale = glm::vec3(0.01f);

    std::cerr << "Adding GameBoyClassic to scene" << std::endl;
    engine.GetScene().AddGameObject(std::move(gameBoyClassic));

    std::cerr << "Starting engine run" << std::endl;
    engine.Run();
    return 0;
}
