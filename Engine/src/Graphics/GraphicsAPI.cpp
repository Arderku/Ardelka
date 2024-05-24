#include "GraphicsAPI.h"
//#include "OpenGLGraphicsAPI.h" // Include when implemented
//#include "VulkanGraphicsAPI.h" // Include when implemented
//#include "DirectX12GraphicsAPI.h" // Include when implemented
//#include "WebGLGraphicsAPI.h" // Include when implemented

GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::None;

std::unique_ptr<GraphicsAPI> GraphicsAPI::Create() {
    switch (s_API) {
        case GraphicsAPI::API::None:    throw std::runtime_error("GraphicsAPI::None is currently not supported!");
      //  case GraphicsAPI::API::OpenGL:  return std::make_unique<OpenGLGraphicsAPI>();
        //case GraphicsAPI::API::Vulkan:  return std::make_unique<VulkanGraphicsAPI>();
      //  case GraphicsAPI::API::DirectX12: return std::make_unique<DirectX12GraphicsAPI>();
        //case GraphicsAPI::API::WebGL:   return std::make_unique<WebGLGraphicsAPI>();
    }
    throw std::runtime_error("Unknown GraphicsAPI!");
}
