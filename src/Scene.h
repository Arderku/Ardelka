#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"

class Scene {
public:
    void Update();
    void Render();
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};
