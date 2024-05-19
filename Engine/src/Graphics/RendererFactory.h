#pragma once

#include "RendererType.h"
#include "Renderer.h"

class RendererFactory {
public:
    static Renderer* CreateRenderer(RendererType type);
};

