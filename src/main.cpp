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

int main() {
    Engine engine;
    engine.Init();
    std::cerr << "Starting main" << std::endl;

    std::cerr << "Creating Shader" << std::endl;
    Shader* shader = new Shader("resources/shaders/vertex_shader.glsl", "resources/shaders/fragment_shader.glsl");
    if (!shader || !shader->IsValid()) {
        std::cerr << "Shader creation failed" << std::endl;
        return -1;
    }

    std::cerr << "Creating Material" << std::endl;
    Material* material = new Material(shader);

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
    parentGameObject->GetTransform()->position = glm::vec3(0.0f, 1.0f, 0.0f);
    parentGameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, material));

    std::cerr << "Creating child GameObject" << std::endl;
    auto childGameObject = std::make_unique<GameObject>();
    childGameObject->AddComponent(std::make_unique<MeshRenderer>(mesh, material));
    childGameObject->GetTransform()->position = glm::vec3(0.0f, 2.0f, 0.0f);
    parentGameObject->AddChild(std::move(childGameObject));

    std::cerr << "Adding GameObject to scene" << std::endl;
    engine.GetScene().AddGameObject(std::move(parentGameObject));

    std::cerr << "Starting engine run" << std::endl;
    engine.Run();
    return 0;
}
