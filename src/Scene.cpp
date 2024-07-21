#include <iostream>
#include "Scene.h"

void Scene::Update() {
    for (auto& gameObject : gameObjects) {
        gameObject->Update();
    }
}

void Scene::Render() {
    //log gameObjects count
    for (auto& gameObject : gameObjects) {

        gameObject->Render();
    }
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
    gameObjects.push_back(std::move(gameObject));
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const {
    return gameObjects;
}

std::vector<std::unique_ptr<GameObject>> &Scene::GetGameObjects() {
    return gameObjects;
}
