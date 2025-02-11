//
// Created by arder on 2/12/2025.
//

#ifndef GAMELAYER_H
#define GAMELAYER_H

#endif //GAMELAYER_H

#pragma once
#include "Layer.h"
#include "Scene.h"
#include "Renderer.h"

class GameLayer : public Layer {
public:
    GameLayer(Scene& scene, Renderer& renderer)
        : m_Scene(scene), m_Renderer(renderer)
    {
        m_Name = "GameLayer";
    }

    virtual void OnUpdate(float deltaTime) override {
        // Update the scene and render it.
        m_Scene.Update();
        m_Renderer.Render(m_Scene);
    }

private:
    Scene& m_Scene;
    Renderer& m_Renderer;
};
