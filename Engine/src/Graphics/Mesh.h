#pragma once

#include <vector>
#include "Vertex.h"

class Mesh {
public:
    virtual ~Mesh() = default;

    virtual void SetVertices(const std::vector<Vertex>& vertices) = 0;

    virtual void SetIndices(const std::vector<uint32_t>& indices) = 0;

    // Binds the mesh (vertex and index buffers) for rendering
    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    virtual void Draw() const = 0;
};
