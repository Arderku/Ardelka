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

int main() {
    Engine engine;
    engine.Init();
    std::cerr << "Starting main" << std::endl;

    std::cerr << "Creating Shader" << std::endl;

    engine.GetRenderer().GetShader();

    std::cerr << "Creating Material" << std::endl;
    Texture* albedo = ResourceManager::loadTexture("albedo", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_albedo.png");
    Texture* metallic = ResourceManager::loadTexture("metallic", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_metallic.png");
    Texture* roughness = ResourceManager::loadTexture("roughness", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_roughness.png");
    Texture* normal = ResourceManager::loadTexture("normal", "resources/DummyAssets/Laminate-Flooring-brown/laminate-flooring-brown_normal.png");

    Material* materialRed = new Material(engine.GetRenderer().GetShader());
    materialRed->setBaseColor(glm::vec4(1.5f, 0.5f, 0.5f, 1.0f));
    materialRed->setBaseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    materialRed->setMetallic(0.5f);
    materialRed->setRoughness(0.15f);

    materialRed->setAlbedoMap(albedo);
    materialRed->setMetallicMap(metallic);
    materialRed->setRoughnessMap(roughness);
    materialRed->setNormalMap(normal);

    Material* materialBlue = new Material(engine.GetRenderer().GetShader());
    materialBlue->setBaseColor(glm::vec4(.5f, 0.5f, 1.5f, 1.0f));
    //materialBlue->setAlbedoMap(albedo);

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
    childGameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialBlue));
    childGameObject->GetTransform()->position = glm::vec3(2.0f, 1.0f, 0.0f);
    childGameObject->GetTransform()->scale = glm::vec3(2.f, 1.f, 0.5f);

    parentGameObject->AddChild(std::move(childGameObject));

    /*
    //creat 100 game objects at random positions
    for (int i = 0; i < 100; i++) {
        auto gameObject = std::make_unique<GameObject>();
        gameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, materialRed));
        gameObject->GetTransform()->position = glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5,
                                                         static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5,
                                                         static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 7 - 5);
        gameObject->GetTransform()->scale = glm::vec3(0.5f);
        engine.GetScene().AddGameObject(std::move(gameObject));
    }
    */

    std::cerr << "Adding GameObject to scene" << std::endl;
    engine.GetScene().AddGameObject(std::move(parentGameObject));

    std::cerr << "Starting engine run" << std::endl;
    engine.Run();
    return 0;
}
