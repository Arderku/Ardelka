#pragma once

#include "Core/Window.h"
#include <memory>

enum class GraphicsAPI {
    None = 0,
    Vulkan = 1,
    // Add other APIs as needed
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void Initialize() = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void SwapBuffers() = 0;

    static std::shared_ptr<Renderer> Create(Window& window, GraphicsAPI api);

    static GraphicsAPI GetAPI() { return s_GraphicsAPI; }

private:
    static GraphicsAPI s_GraphicsAPI;
};
