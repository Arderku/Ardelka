#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
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
    Application application;
    application.Init();
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

        auto tt = ResourceManager::GetDefaultWhiteTexture();
    materialRed->SetAlbedoMap(tt);
    materialRed->SetMetallicMap(metallic);
    materialRed->SetRoughnessMap(roughness);
    materialRed->SetNormalMap(normal);

        materialBlue->SetAlbedoMap(albedo);

    // Create a GameObject with a Camera component
    auto cameraGameObject = std::make_unique<GameObject>();
    cameraGameObject->SetName("MainCamera");
    cameraGameObject->AddComponent(std::make_unique<Camera>(60.0f, 2.0f / 1.0f, 0.1f, 1000.0f));
    cameraGameObject->GetTransform()->position = glm::vec3(0.0f, 2.0f, -5.0f);
    cameraGameObject->GetTransform()->rotation = glm::vec3(20.0f, 0.0f, 0.0f);

    application.GetScene().SetActiveCamera(cameraGameObject->GetComponent<Camera>());
    application.GetScene().AddGameObject(std::move(cameraGameObject));

    // Define vertices and indices for a plane
    std::vector<float> planeVertices = {
            // positions          // normals           // texcoords
            -5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            5.0f,  0.0f, -5.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            -5.0f,  0.0f,  5.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };
    std::vector<unsigned int> planeIndices = {
            0, 1, 2, 2, 3, 0,
    };

    // Define vertices and indices for a cube
    std::vector<float> cubeVertices = {
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

    std::vector<unsigned int> cubeIndices = {
            0, 1, 2, 2, 3, 0,      // Front face
            4, 5, 6, 6, 7, 4,      // Back face
            8, 9, 10, 10, 11, 8,   // Left face
            12, 13, 14, 14, 15, 12,// Right face
            16, 17, 18, 18, 19, 16,// Bottom face
            20, 21, 22, 22, 23, 20 // Top face
    };

    // Create mesh for plane
    std::cerr << "Creating Plane Mesh" << std::endl;
    auto planeMesh = std::make_shared<Mesh>(planeVertices, planeIndices);

    // Create mesh for cube
    std::cerr << "Creating Cube Mesh" << std::endl;
    auto cubeMesh = std::make_shared<Mesh>(cubeVertices, cubeIndices);

    // Create the plane GameObject
    std::cerr << "Creating Plane GameObject" << std::endl;
    auto planeGameObject = std::make_unique<GameObject>();
    planeGameObject->SetName("Plane");
    planeGameObject->GetTransform()->position = glm::vec3(0.0f, -1.5f, 0.0f);
    planeGameObject->GetTransform()->scale = glm::vec3(1.0f);
    planeGameObject->AddComponent(std::make_unique<MeshRenderer>(planeMesh, materialRed));

    std::cerr << "Adding Plane GameObject to scene" << std::endl;
    application.GetScene().AddGameObject(std::move(planeGameObject));

    // Create cubes around the GameBoyClassic model
    std::vector<glm::vec3> cubePositions = {
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, -2.0f)
    };

    for (const auto& position : cubePositions) {
        auto cubeGameObject = std::make_unique<GameObject>();
        cubeGameObject->SetName("Cube" );
        cubeGameObject->GetTransform()->position = position;
        cubeGameObject->GetTransform()->scale = glm::vec3(0.5f);
        cubeGameObject->AddComponent(std::make_unique<MeshRenderer>(cubeMesh, materialBlue));
        application.GetScene().AddGameObject(std::move(cubeGameObject));
    }

    // Load the GameBoyClassic model
    std::cerr << "Loading GameBoyClassic model" << std::endl;
    auto gameBoyClassic = ResourceManager::loadModel("Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic.fbx", pbrShader);

    auto GameBoyAlbedo = ResourceManager::loadTexture("GameBoyAlbedo", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_BaseColor.png");
    auto GameBoyMetallic = ResourceManager::loadTexture("GameBoyMetallic", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Metallic.png");
    //auto GameBoyRoughness = ResourceManager::loadTexture("GameBoyRoughness", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Roughness.png");
    auto GameBoyNormal = ResourceManager::loadTexture("GameBoyNormal", "Resources/DummyAssets/Models/GameBoyClassic/GameBoyClassic_Normal.png");

    auto GameBoyMaterial = ResourceManager::createMaterial("GameBoyClassic", pbrShader);
    GameBoyMaterial->SetBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    GameBoyMaterial->SetAlbedoMap(GameBoyAlbedo);
    GameBoyMaterial->SetMetallicMap(GameBoyMetallic);
   // GameBoyMaterial->SetRoughnessMap(GameBoyRoughness);
    GameBoyMaterial->SetNormalMap(GameBoyNormal);

    auto gameBoyRenderer = gameBoyClassic->GetComponent<MeshRenderer>();
    gameBoyRenderer->SetMaterial(GameBoyMaterial);

    gameBoyClassic->GetTransform()->position = glm::vec3(0.0f, -1.0f, 0.0f);
    gameBoyClassic->GetTransform()->rotation = glm::vec3(0.0f, 45.0f, 0.0f);
    gameBoyClassic->GetTransform()->scale = glm::vec3(0.01f);

    std::cerr << "Adding GameBoyClassic to scene" << std::endl;
    application.GetScene().AddGameObject(std::move(gameBoyClassic));



    std::cerr << "Starting engine run" << std::endl;
    application.Run();
    return 0;
}
