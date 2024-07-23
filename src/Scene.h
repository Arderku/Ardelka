#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Camera.h"

class Scene {
public:
    void Update();
    void Render();
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

    // Get and set active camera
    Camera* GetActiveCamera() const { return m_ActiveCamera; }
    void SetActiveCamera(Camera* camera) { m_ActiveCamera = camera; }

    //get GameObjects
    std::vector<std::unique_ptr<GameObject>>& GetGameObjects();

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    Camera* m_ActiveCamera = nullptr;

    void Init();
};
