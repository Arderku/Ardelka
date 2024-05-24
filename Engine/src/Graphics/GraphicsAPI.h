#pragma once
#include <glm/glm.hpp>
#include <memory>

class GraphicsAPI {
public:
    enum class API {
        None = 0, Vulkan = 1, OpenGL = 2, DirectX12 = 3, WebGL = 4
    };

    virtual ~GraphicsAPI() = default;

    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    static API GetAPI() { return s_API; }
    static std::unique_ptr<GraphicsAPI> Create();

private:
    static API s_API;
};

