#include "RendererFactory.h"
#include "GraphicsFactory.h"

Renderer* RendererFactory::CreateRenderer(RendererType type) {
    return GraphicsFactory::CreateRenderer(type);
}
